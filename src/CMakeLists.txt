add_library(Message Msg/Message.cpp)
target_link_libraries(Message PUBLIC simplerpc_common)

add_library(Client Client/Client.cpp)
target_link_libraries(Client PUBLIC Message simplerpc_common)

add_library(Server Server/Server.cpp)
target_link_libraries(Server PUBLIC Message simplerpc_common)