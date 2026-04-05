#!/bin/bash
set -e

# ---------------------------------------------------------------------------
# setup.sh — check and install build dependencies for SimpleRPC
# Required: build-essential, g++-14, cmake (>=3.22.1), libfmt-dev, clang-format
# ---------------------------------------------------------------------------

if [ "$(id -u)" -eq 0 ]; then
    SUDO=""
else
    if ! command -v sudo &>/dev/null; then
        echo "ERROR: This script must be run as root or with sudo available." >&2
        exit 1
    fi
    SUDO="sudo"
fi

# Install a package via apt if it is not already installed (dpkg check)
ensure_pkg() {
    local pkg="$1"
    if dpkg -s "$pkg" &>/dev/null 2>&1; then
        echo "[OK]      $pkg is already installed"
    else
        echo "[INSTALL] Installing $pkg ..."
        $SUDO apt-get install -y "$pkg"
    fi
}

# Install a package via apt if a binary is not already on PATH
ensure_bin() {
    local bin="$1"
    local pkg="${2:-$1}"
    if command -v "$bin" &>/dev/null; then
        echo "[OK]      $bin is already available ($(command -v "$bin"))"
    else
        echo "[INSTALL] $bin not found — installing $pkg ..."
        $SUDO apt-get install -y "$pkg"
    fi
}

# ---------------------------------------------------------------------------
echo "==> Updating package lists ..."
$SUDO apt-get update -qq

# ---------------------------------------------------------------------------
echo ""
echo "==> Checking build dependencies ..."

ensure_pkg build-essential
ensure_bin g++-14 g++-14
ensure_pkg libfmt-dev
ensure_bin clang-format clang-format

# cmake — install first, then verify version meets the project minimum
ensure_bin cmake cmake

CMAKE_MIN="3.22.1"
CMAKE_VER=$(cmake --version | head -1 | sed 's/cmake version //')

version_ge() {
    # Returns 0 (true) if $1 >= $2 in semver order
    printf '%s\n%s\n' "$2" "$1" | sort -V -C
}

echo ""
echo "==> Checking cmake version (need >= $CMAKE_MIN, found $CMAKE_VER) ..."
if version_ge "$CMAKE_VER" "$CMAKE_MIN"; then
    echo "[OK]      cmake $CMAKE_VER satisfies >= $CMAKE_MIN"
else
    echo "[WARN]    cmake $CMAKE_VER is older than required $CMAKE_MIN." >&2
    echo "          Consider installing a newer cmake from https://cmake.org/download/" \
         "or via the Kitware apt repository." >&2
fi

# ---------------------------------------------------------------------------
echo ""
echo "==> All checks complete."
echo "    Environment ready. Run ./builder.sh to build."
