#!/bin/sh
set -e

dir=$PWD

rm -rf build build_logging
mkdir -p build build_logging

cd "$dir/build"
cmake ..
cmake --build .
cd ..

cd "$dir/build_logging"
cmake -DSIMPLERPC_ENABLE_LOGGING=ON ..
cmake --build .
cd ..
