#pragma once
#include <type_traits>

//https://www.studyplan.dev/pro-cpp/type-traits

template<typename T>
struct is_serializable:std::false_type{};

//template<>
//struct is_serializable<std::string>:std::true_type{};