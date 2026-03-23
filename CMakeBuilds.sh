#!/bin/sh
set -e

dir=$PWD
build="build"
build_logging="build_logging"
build_debugging="build_debugging"
build_debugging_threads="build_debugging_threads"
build_debugging_address="build_debugging_address"
release="release"

rm -rf  $build $build_logging $build_debugging $build_debugging_threads $build_debugging_address $release
mkdir -p $build $build_logging $build_debugging $build_debugging_threads $build_debugging_address $release

cd "$dir/$build"
cmake ..
cmake --build .
cd ..

cd "$dir/$build_logging"
cmake -DSIMPLERPC_ENABLE_LOGGING=ON ..
cmake --build .
cd ..

cd "$dir/$build_debugging"
cmake -DSIMPLERPC_ENABLE_LOGGING=ON -DSIMPLERPC_DEBUGGING=ON ..
cmake --build .
cd ..

cd "$dir/$build_debugging_threads"
cmake -DSIMPLERPC_ENABLE_LOGGING=ON -DSIMPLERPC_DEBUGGING=ON -DSIMPLERPC_THREAD_SANITIZER=ON ..
cmake --build .
cd ..

cd "$dir/$build_debugging_address"
cmake -DSIMPLERPC_ENABLE_LOGGING=ON -DSIMPLERPC_DEBUGGING=ON -DSIMPLERPC_ADDRESS_SANITIZER=ON ..
cmake --build .
cd ..

cd "$dir/$release"
cmake -DSIMPLERPC_RELEASE=ON ..
cmake --build .
cd ..