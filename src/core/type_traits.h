#ifndef TYPE_TRAITS_H
#define TYPE_TRAITS_H

#include <tuple>
#include <typeinfo>
#include <type_traits>
#include <boost/type_traits.hpp> // for has_equal_to
#include <boost/mpl/equal.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/has_xxx.hpp>

//! Failback for template specialization test.
template <template <typename...> class Template, typename T>
struct is_specialization_of : std::false_type {};

//! Tests whether a given type is a specialization of Template.
template <template <typename...> class Template, typename... Args>
struct is_specialization_of<Template, Template<Args...>> : std::true_type {};

// Expands into a unary metafunction that determines if the specified 
// type has a nested typedef ::value_type.
BOOST_MPL_HAS_XXX_TRAIT_DEF(value_type);

template <typename T>
struct is_piecewise_comparable;

template <typename T, size_t N>
struct is_piecewise_comparable<std::array<T, N>> : 
  boost::has_equal_to<T, T, bool>::type {};

template <template <typename...> class T, typename... Args> 
struct is_piecewise_comparable<T<Args...>> {
  typedef typename boost::mpl::vector<Args...> type_list;
  typedef typename boost::mpl::equal<
    type_list,
    type_list,
    boost::has_equal_to<boost::mpl::_1, boost::mpl::_2>
  >::type type;
};

//! Failback for std::array detection.
template <typename T>
struct is_std_array : std::false_type {};

//! Tests whether a given type is an std::array.
template <typename T, size_t N>
struct is_std_array<std::array<T, N>> : std::true_type {};

//! 
template <class A, class B>
struct either : std::integral_constant<bool, A::value || B::value> {};

template <class A, class B>
struct both : std::integral_constant<bool, A::value && B::value> {};

template <typename T>
struct is_static_container : either<
  is_std_array<T>, is_specialization_of<std::tuple, T>
> {};

//! Constexpr to evaluate the size of variable size collections uniformly. 
template <typename T, size_t N>
constexpr size_t countof(T const(&)[N]) { return N; };

//! Type catch-all to facilitate SFINAE via declval substitution.
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
