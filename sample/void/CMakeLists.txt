add_executable(void_client client.cpp)
target_link_libraries(void_client PRIVATE Message Client simplerpc_common)

add_executable(void_server server.cpp)
target_link_libraries(void_server PRIVATE Message Server simplerpc_common)