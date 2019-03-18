//
// Created by Maksym Maisak on 2019-01-09.
//

#ifndef SAXION_Y2Q2_RENDERING_TUPLEUTILS_H
#define SAXION_Y2Q2_RENDERING_TUPLEUTILS_H

#include <tuple>
#include <type_traits>

template<typename T, typename Tuple>
struct has_type;

template<typename T>
struct has_type<T, std::tuple<>> : std::false_type {};

template<typename T, typename U, typename... Rest>
struct has_type<T, std::tuple<U, Rest...>> : has_type<T, std::tuple<Rest...>> {};

template<typename T, typename... Rest>
struct has_type<T, std::tuple<T, Rest...>> : std::true_type {};

template<typename T, typename Tuple>
inline constexpr bool has_type_v = has_type<T, Tuple>::value;

#endif //SAXION_Y2Q2_RENDERING_TUPLEUTILS_H
