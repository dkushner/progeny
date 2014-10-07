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

//! Constexpr to evaluate the size of variable size collections uniformly. 
template <typename T, size_t N>
constexpr size_t countof(T const(&)[N]) { return N; };

//! Type catch-all to mediate SFINAE via declval substitution.
/*!
 *  Essentially, we use this so that if our substitution for 
 *  decltype(std::declval<T>.size()) fails, we fall back on a 
 *  valid type and maintain SFINAE. 
 */
template <typename T>
struct type_void { typedef void type; };

//! Failure specialization.
template <typename T, typename = void>
struct has_size : std::false_type {};

//! Success specialization.
template <typename T>
struct has_size<T, type_void<decltype(std::declval<T>().size())> > :
  std::true_type {};

//! Constexpr to evaluate the size of 
template <typename T>
constexpr std::enable_if<has_size<const T&>::value, size_t> countof(const T& t) {
  return t.size();
}

// Get that weak pointer shit out of my house.
template <typename T>
constexpr std::enable_if<std::is_pointer<T>::value> countof(const T& t) = delete;

#endif
