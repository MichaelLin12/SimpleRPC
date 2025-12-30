cmake_minimum_required(VERSION 3.22.1)
project(MyRPCSys VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(SIMPLERPC_COMPILE_DEFINITIONS "")

option(SIMPLERPC_ENABLE_LOGGING "Enable logging" OFF)

if(SIMPLERPC_ENABLE_LOGGING)
    list(APPEND SIMPLERPC_COMPILE_DEFINITIONS LOGGING)
endif()

add_library(Message src/Msg/Message.cpp)
target_include_directories(Message PUBLIC include)
target_compile_definitions(Message PUBLIC ${SIMPLERPC_COMPILE_DEFINITIONS})

add_library(Client src/Client/Client.cpp)
target_include_directories(Client PUBLIC include)
target_link_libraries(Client PUBLIC Message)
target_compile_definitions(Client PUBLIC ${SIMPLERPC_COMPILE_DEFINITIONS})

add_library(Server src/Server/Server.cpp)
target_include_directories(Server PUBLIC include)
target_link_libraries(Server PUBLIC Message)
target_compile_definitions(Server PUBLIC ${SIMPLERPC_COMPILE_DEFINITIONS})

add_executable(client client.cpp)
target_include_directories(client PUBLIC include)
target_link_libraries(client PUBLIC Message Client)
target_compile_definitions(client PUBLIC ${SIMPLERPC_COMPILE_DEFINITIONS})

add_executable(server server.cpp)
target_include_directories(server PUBLIC include)
target_link_libraries(server PUBLIC Message Server)
target_compile_definitions(server PUBLIC ${SIMPLERPC_COMPILE_DEFINITIONS})