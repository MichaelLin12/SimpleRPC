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
    local BOOST_FOLDER="/usr/include/boost"
    if [[ ! -e "$BOOST_FOLDER" ]]; then
        printf "%s\n" "Boost library does not exist"
        return 1
    fi
}

check_fmt(){
    printf "%s\n" "Checking if fmt exists"
    local FMT_LIBRARY="/usr/include/fmt"
    if [[ ! -e "$FMT_LIBRARY" ]]; then
        printf "%s\n" "fmt library does not exist"
        return 1
    fi
}


check_clang(){
    printf "%s\n" "Checking if clang++ exists"
    if [[ ! $(command -v clang++) ]]; then
        printf "%s\n" "clang++ does not exist"
        return 1
    fi
}

check_gcc(){
    printf "%s\n" "Checking if g++ exists"
    if [[ ! $(command -v g++) ]]; then
        printf "%s\n" "g++ does not exist"
        return 1
    fi
}

check_cmake(){
    printf "%s\n" "Checking if cmake exists"
    if [[ ! $(command -v cmake) ]]; then
        printf "%s\n" "cmake doesn't exist"
        return 1
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
    printf "%s\n" "Installing clang++"
    apt update &> /dev/null && apt upgrade &> /dev/null && apt install -y clang  &> /dev/null
    printf "%s\n" "clang++ been successfully installed"
}

install_gcc(){
    printf "%s\n" "Installing gcc"
    apt update &> /dev/null && apt upgrade &> /dev/null && apt install -y build-essentials &> /dev/null
    printf "%s\n" "gcc has been successfully installed"
}

install_cmake(){
    print "%s\n" "Installing cmake"
    apt update &> /dev/null
    apt upgrade &> /dev/null
    apt install -y cmake &> /dev/null
    printf"%s\n" "CMake has successfully been installed"
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