cmake_minimum_required(VERSION 3.22.1)
project(MyRPCSys VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-g)

add_library(Message src/Msg/Message.cpp)
target_include_directories(Message PUBLIC include)

add_library(Client src/Client/Client.cpp)
target_include_directories(Client PUBLIC include)
target_link_libraries(Client PUBLIC Message)

add_library(Server src/Server/Server.cpp)
target_include_directories(Server PUBLIC include)

add_executable(client client.cpp)
target_include_directories(client PUBLIC include)
target_link_libraries(client PUBLIC Message Client)

add_executable(server server.cpp)
target_include_directories(server PUBLIC include)
target_link_libraries(server PUBLIC Message Server)