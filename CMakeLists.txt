cmake_minimum_required(VERSION 3.22.1)
project(MyRPCSys VERSION 1.0 LANGUAGES CXX)

option(SIMPLERPC_ENABLE_LOGGING "Enable logging" OFF)
option(SIMPLERPC_BUILD_SAMPLES "Build sample programs" ON)

# Interface library — shared settings for all targets
add_library(simplerpc_common INTERFACE)
target_compile_features(simplerpc_common INTERFACE cxx_std_23)
target_include_directories(simplerpc_common INTERFACE include)

if(SIMPLERPC_ENABLE_LOGGING)
    target_compile_definitions(simplerpc_common INTERFACE ENABLELOGGING)
endif()

add_subdirectory(src)

if(SIMPLERPC_BUILD_SAMPLES)
    add_subdirectory(sample)
endif()