#ifndef TYPE_TRAITS_H
#define TYPE_TRAITS_H

#include <tuple>
#include <type_traits>

template <template <typename...> class Template, typename T>
struct is_specialization_of : std::false_type {};

template <template <typename...> class Template, typename... Args>
struct is_specialization_of<Template, Template<Args...>> : std::true_type {};

template <typename T>
struct is_std_array : std::false_type {};

template <typename T, size_t N>
struct is_std_array<std::array<T, N>> : std::true_type {};

template <class A, class B>
struct either : std::integral_constant<bool, A::value || B::value> {};

template <typename T>
struct is_static_container : either<
  is_std_array<T>, is_specialization_of<std::tuple, T>
> {};

#endif
