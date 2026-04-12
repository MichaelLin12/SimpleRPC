add_executable(add_client client.cpp)
target_link_libraries(add_client PRIVATE Message Client simplerpc_common)

add_executable(add_server server.cpp)
target_link_libraries(add_server PRIVATE Message Server simplerpc_common)