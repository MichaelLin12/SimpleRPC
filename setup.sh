#!/usr/bin/bash

set -euo pipefail

check_root(){
    if [[ "$(whoami)" != "root" ]]; then
        printf "%s\n" "Please run as root or with sudo"
        return 1
    fi
}

check_boost(){
    printf "%s\n" "Checking if the boost library exists"
    local VERSION_BOOST="$(dpkg -l | grep libboost-dev | awk '{print $3}' | cut -d'.' -f1,2)"
    if [[ -z "$VERSION_BOOST" ]]; then
        printf "%s\n" "Boost library does not exist"
        return 1
    fi

    local VERSION="$(printf "%s\n" "1.83" "$VERSION_BOOST" | sort -V | head -n1)"
    if [[ "$VERSION" != "1.83" ]]; then
        printf "%s\n" "Boost library is too old"
        return 1
    else
        printf "%s\n" "Boost library is acceptable"
    fi
}

check_fmt(){
    printf "%s\n" "Checking if fmt exists"
    local VERSION_FMT="$(dpkg -l | grep libfmt-dev | awk '{print $3}' | cut -d'.' -f1)"
    if [[ -z "$VERSION_FMT" ]]; then
        printf "%s\n" "fmt library does not exist"
        return 1
    fi

    local VERSION="$(printf "%s\n" "12" "$VERSION_FMT" | sort -V | head -n1)"
    if [[ "$VERSION" != "12" ]]; then
        printf "%s\n" "fmt library is too old"
        return 1
    else
        printf "%s\n" "fmt library is acceptable"
    fi
}


check_clang(){
    printf "%s\n" "Checking if clang++ exists"
    if [[ ! $(command -v clang++) ]]; then
        printf "%s\n" "clang++ does not exist"
        return 1
    fi

    local VERSION_CLANG="$(clang++ --version | head -n1 | awk '{print $3}' | cut -d'.' -f1)"
    local VERSION="$(printf "%s\n" "18" "$VERSION_CLANG" | sort -V | head -n1)"
    if [[ "$VERSION" != "18" ]]; then
        printf "%s\n" "clang++ is too old"
        return 1
    else
        printf "%s\n" "clang++ is acceptable"
    fi
}

check_gcc(){
    printf "%s\n" "Checking if g++ exists"
    if [[ ! $(command -v g++) ]]; then
        printf "%s\n" "g++ does not exist"
        return 1
    fi

    local VERSION_GCC="$(g++ --version | head -n1 | awk '{print $3}' | cut -d'.' -f1)"
    local VERSION="$(printf "%s\n" "14" "$VERSION_GCC" | sort -V | head -n1)"
    if [[ "$VERSION" != "14" ]]; then
        printf "%s\n" "g++ is too old"
        return 1
    else
        printf "%s\n" "g++ is acceptable"
    fi
}

install_boost(){
    printf "%s\n" "Installing Boost"
    git clone --depth=1 --branch boost-1.83.0 https://github.com/boostorg/boost.git >/dev/null 2>&1
    ( cd boost && git submodule update --init >/dev/null 2>&1 )
    mkdir -p /usr/include/boost
    cp -rf boost/libs/*/include/boost/* /usr/include/boost/ >/dev/null 2>&1
    rm -rf boost
    printf "%s\n" "Boost has been successfully installed"
}

install_fmt(){
    printf "%s\n" "Installing fmt"
    git clone --depth=1 --branch 12.0.0 https://github.com/fmtlib/fmt.git >/dev/null 2>&1
    mkdir -p /usr/include/fmt
    cp -rf fmt/include/fmt/* /usr/include/fmt/ >/dev/null 2>&1
    rm -rf fmt
    printf "%s\n" "fmt has been successfully installed"
}

install_clang(){
    printf "%s\n" "Installing clang-18"
    wget https://apt.llvm.org/llvm.sh >/dev/null 2>&1
    chmod +x llvm.sh
    ./llvm.sh 18 >/dev/null 2>&1
    rm llvm.sh
    printf "%s\n" "clang-18 has been successfully installed"
}

install_clang(){
    printf "%s\n" "Installing clang++-18"
    wget https://apt.llvm.org/llvm.sh >/dev/null 2>&1
    chmod +x llvm.sh
    ./llvm.sh 18 >/dev/null 2>&1
    rm llvm.sh
    update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-18 100 >/dev/null 2>&1
    printf "%s\n" "clang++-18 has been successfully installed"
}

install_gcc(){
    printf "%s\n" "Installing gcc-14"
    add-apt-repository -y ppa:ubuntu-toolchain-r/test >/dev/null 2>&1
    apt update >/dev/null 2>&1
    apt install -y g++-14 >/dev/null 2>&1
    update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-14 100 >/dev/null 2>&1
    printf "%s\n" "gcc-14 has been successfully installed"
}

main(){
    check_root   || exit 1
    check_boost  || install_boost
    check_fmt    || install_fmt
    check_clang  || install_clang
    check_gcc    || install_gcc
    check_cmake  || install_cmake
    printf "%s\n" "All dependencies are installed"
}

main