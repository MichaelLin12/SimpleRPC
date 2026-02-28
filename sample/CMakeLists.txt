add_executable(client add/client.cpp)
target_link_libraries(client PRIVATE Message Client simplerpc_common)

add_executable(server add/server.cpp)
target_link_libraries(server PRIVATE Message Server simplerpc_common)