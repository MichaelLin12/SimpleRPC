cmake_minimum_required(VERSION 3.22.1)
project(SimpleRPC LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(Message STATIC src/Msg/Message.cpp)
target_include_directories(Message PUBLIC include)

add_executable(test_msg_add_integral tests/test_msg_add_integral.cpp)
target_link_libraries(test_msg_add_integral PRIVATE Message)

add_executable(test_msg_add_string tests/test_msg_add_string.cpp)
target_link_libraries(test_msg_add_string PRIVATE Message)