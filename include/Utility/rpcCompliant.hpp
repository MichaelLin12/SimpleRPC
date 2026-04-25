#pragma once
#include <Msg/Message.hpp>
#include <Msg/MessageTypes.hpp>
#include <concepts>
#include <span>

template <typename T>
concept rpcCompliant = requires(T x, Message& m) {
    requires std::is_class_v<T>;
    { x.serialize(m) } -> std::same_as<void>;
    { x.deserialize(m) } -> std::same_as<T>;
    { x.size() } -> std::same_as<std::size_t>;
    requires !requires { x.serialize(std::declval<Message&&>()); };
    requires !requires { x.deserialize(std::declval<Message&&>()); };
};
