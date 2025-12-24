cmake_minimum_required(VERSION 3.22.1)
project(SimpleRPC LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(Message STATIC src/Msg/Message.cpp)
target_include_directories(Message PUBLIC include)

add_library(Serializer STATIC src/Codec/Serializer.cpp)
target_include_directories(Serializer PUBLIC include)

add_library(Deserializer STATIC src/Codec/Deserializer.cpp)
target_include_directories(Deserializer PUBLIC include)

add_executable(test_msg_add_integral tests/test_msg_add_integral.cpp)
target_compile_options(test_msg_add_integral PUBLIC -O0 -fsanitize=address -g -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion -Werror)
target_link_options(test_msg_add_integral PUBLIC -fsanitize=address)
target_link_libraries(test_msg_add_integral PUBLIC Message)

add_executable(test_msg_add_string tests/test_msg_add_string.cpp)
target_compile_options(test_msg_add_string PUBLIC -O0 -fsanitize=address -g -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion -Werror)
target_link_options(test_msg_add_string PUBLIC -fsanitize=address)
target_link_libraries(test_msg_add_string PUBLIC Message)


add_executable(test_serializer tests/test_serializer.cpp)
target_compile_options(test_serializer PUBLIC -O0 -fsanitize=address -g -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion -Werror)
target_link_options(test_serializer PUBLIC -fsanitize=address)
target_link_libraries(test_serializer PUBLIC Serializer Message)

add_executable(test_encoder_integral tests/test_encoder_integral.cpp)
target_compile_options(test_encoder_integral PUBLIC -O0 -fsanitize=address -g -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion -Werror)
target_link_options(test_encoder_integral PUBLIC -fsanitize=address)
target_link_libraries(test_encoder_integral PUBLIC Message)
target_include_directories(test_encoder_integral PUBLIC include)

add_executable(test_encoder_byte tests/test_encoder_byte.cpp)
target_compile_options(test_encoder_byte PUBLIC -O0 -fsanitize=address -g -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion -Werror)
target_link_options(test_encoder_byte PUBLIC -fsanitize=address)
target_link_libraries(test_encoder_byte PUBLIC Message)
target_include_directories(test_encoder_byte PUBLIC include)

add_executable(test_encoder_string tests/test_encoder_string.cpp)
target_compile_options(test_encoder_string PUBLIC -O0 -fsanitize=address -g -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion -Werror)
target_link_options(test_encoder_string PUBLIC -fsanitize=address)
target_link_libraries(test_encoder_string PUBLIC Message)
target_include_directories(test_encoder_string PUBLIC include)

add_executable(test_deserialize_int tests/test_deserialize_int.cpp)
target_compile_options(test_deserialize_int PUBLIC -O0 -fsanitize=address -g -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion -Werror)
target_link_options(test_deserialize_int PUBLIC -fsanitize=address)
target_link_libraries(test_deserialize_int PUBLIC Deserializer Message)

add_executable(test_deserialize_string tests/test_deserialize_string.cpp)
target_compile_options(test_deserialize_string PUBLIC -O0 -fsanitize=address -g -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion -Werror)
target_link_options(test_deserialize_string PUBLIC -fsanitize=address)
target_link_libraries(test_deserialize_string PUBLIC Deserializer Message)

add_executable(test_deserialize_uuid tests/test_deserialize_uuid.cpp)
target_compile_options(test_deserialize_uuid PUBLIC -O0 -fsanitize=address -g -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion -Werror)
target_link_options(test_deserialize_uuid PUBLIC -fsanitize=address)
target_link_libraries(test_deserialize_uuid PUBLIC Deserializer Message)