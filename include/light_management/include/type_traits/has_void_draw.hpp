#pragma once

#include <iostream>

#if __cplusplus >= 202002L

#include <concepts>

template<typename T>
concept has_void_draw_v = requires(T v)
{
  {
    v.draw(std::cout, 0u)
    } -> std::convertible_to<void>;
};

namespace {
struct non_valid
{};
static_assert(!has_void_draw_v<non_valid>);
struct valid
{
  void draw(std::ostream&, size_t) const {}
};
static_assert(has_void_draw_v<valid>);
}

#else

#include <type_traits>

template<typename T, class Enable>
struct has_void_draw_impl : std::false_type
{
};

template<typename T>
struct has_void_draw_impl<T, decltype(std::declval<T>().draw(std::cout, 0u))>
  : std::true_type
{
};

template<typename T>
struct has_void_draw : has_void_draw_impl<T, void>
{
};

template<class T>
inline constexpr bool has_void_draw_v = has_void_draw<T>::value;

#endif
