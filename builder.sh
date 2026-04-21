#!/bin/bash
set -euo pipefail

dir=$PWD

build(){
    local name=$1
    local flags=$2

    rm -rf "$name"
    mkdir -p "$name"
    cd "$dir/$name"
    cmake $flags -DCMAKE_CXX_COMPILER=g++-14 ..
    cmake --build .
    cd "$dir"
}

select_build(){
    printf "%s\n" "Select build type:"
    printf "%s\n" "1) Default"
    printf "%s\n" "2) Logging"
    printf "%s\n" "3) Debugging"
    printf "%s\n" "4) Debugging + Thread Sanitizer"
    printf "%s\n" "5) Debugging + Address Sanitizer"
    printf "%s\n" "6) Release"
    printf "%s\n" "7) All"

    read -r choice

    case $choice in
        1) build "build" "" ;;
        2) build "build_logging" "-DSIMPLERPC_ENABLE_LOGGING=ON" ;;
        3) build "build_debugging" "-DSIMPLERPC_ENABLE_LOGGING=ON -DSIMPLERPC_DEBUGGING=ON" ;;
        4) build "build_debugging_threads" "-DSIMPLERPC_ENABLE_LOGGING=ON -DSIMPLERPC_DEBUGGING=ON -DSIMPLERPC_THREAD_SANITIZER=ON" ;;
        5) build "build_debugging_address" "-DSIMPLERPC_ENABLE_LOGGING=ON -DSIMPLERPC_DEBUGGING=ON -DSIMPLERPC_ADDRESS_SANITIZER=ON" ;;
        6) build "release" "-DSIMPLERPC_RELEASE=ON" ;;
        7) 
            build "build" ""
            build "build_logging" "-DSIMPLERPC_ENABLE_LOGGING=ON"
            build "build_debugging" "-DSIMPLERPC_ENABLE_LOGGING=ON -DSIMPLERPC_DEBUGGING=ON"
            build "build_debugging_threads" "-DSIMPLERPC_ENABLE_LOGGING=ON -DSIMPLERPC_DEBUGGING=ON -DSIMPLERPC_THREAD_SANITIZER=ON"
            build "build_debugging_address" "-DSIMPLERPC_ENABLE_LOGGING=ON -DSIMPLERPC_DEBUGGING=ON -DSIMPLERPC_ADDRESS_SANITIZER=ON"
            build "release" "-DSIMPLERPC_RELEASE=ON"
            ;;
        *) printf "%s\n" "Invalid choice"; exit 1 ;;
    esac
}

main(){
    select_build
}

main