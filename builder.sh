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
cmake -DCMAKE_CXX_COMPILER=g++-14 ..
cmake --build .
cd ..

cd "$dir/$build_logging"
cmake -DSIMPLERPC_ENABLE_LOGGING=ON -DCMAKE_CXX_COMPILER=g++-14 ..
cmake --build .
cd ..

cd "$dir/$build_debugging"
cmake -DSIMPLERPC_ENABLE_LOGGING=ON -DSIMPLERPC_DEBUGGING=ON -DCMAKE_CXX_COMPILER=g++-14 ..
cmake --build .
cd ..

cd "$dir/$build_debugging_threads"
cmake -DSIMPLERPC_ENABLE_LOGGING=ON -DSIMPLERPC_DEBUGGING=ON -DSIMPLERPC_THREAD_SANITIZER=ON -DCMAKE_CXX_COMPILER=g++-14 ..
cmake --build .
cd ..

cd "$dir/$build_debugging_address"
cmake -DSIMPLERPC_ENABLE_LOGGING=ON -DSIMPLERPC_DEBUGGING=ON -DSIMPLERPC_ADDRESS_SANITIZER=ON -DCMAKE_CXX_COMPILER=g++-14 ..
cmake --build .
cd ..

cd "$dir/$release"
cmake -DSIMPLERPC_RELEASE=ON -DCMAKE_CXX_COMPILER=g++-14 ..
cmake --build .
cd ..