#ifndef CL_MPL_H
#define CL_MPL_H

#include <type_traits>
#include <tuple>
#include <array>

template<class...>
struct cl_list {
};

template<template<class...> class F, class... A>
struct cl_apply_impl;

template<template<class...> class A, class... T,
    template<class...> class B>
struct cl_apply_impl<B, A<T...>> {
  using type = B<T...>;
};

template<template<class...> class F, class... L> using cl_apply = typename cl_apply_impl<F, L...>::type;

template<class L>
struct cl_length_impl {
};

template<class... T, template<class...> class F>
struct cl_length_impl<F<T...>> {
  using type = std::integral_constant<std::size_t, sizeof...(T)>;
};

template<class L> using cl_length = typename cl_length_impl<L>::type;

template<class T, class L>
struct cl_cons_impl;

template<template<class...> class L, class... U, class T>
struct cl_cons_impl<T, L<U...>> {
  using type = L<T, U...>;
};

template<class T, class L> using cl_cons = typename cl_cons_impl<T, L>::type;

template<class L>
struct cl_car_cdr_impl {
};

template<template<class...> class L, class... T, class U>
struct cl_car_cdr_impl<L<U, T...>> {
  using car_type = U;
  using cdr_type = L<T...>;
};

template<class L> using cl_car = typename cl_car_cdr_impl<L>::car_type;

template<class L> using cl_cdr = typename cl_car_cdr_impl<L>::cdr_type;

template<class L>
struct cl_lastcar_impl {
};

template<class L> using cl_lastcar = typename cl_lastcar_impl<L>::type;

template<template<class...> class L, class T>
struct cl_lastcar_impl<L<T>> {
  using type = T;
};

template<template<class...> class L, class T, class... U>
struct cl_lastcar_impl<L<T, U...>> {
  using type = cl_lastcar<L<U...>>;
};

template<class L>
struct cl_butlast_impl {
};

template<class L> using cl_butlast = typename cl_butlast_impl<L>::type;

template<template<class...> class L, class T>
struct cl_butlast_impl<L<T>> {
  using type = L<>;
};

template<template<class...> class L, class T, class U>
struct cl_butlast_impl<L<T, U>> {
  using type = L<T>;
};

template<template<class...> class L, class T, class... U>
struct cl_butlast_impl<L<T, U...>> {
  using type = cl_cons<T, cl_butlast<L<U...>>>;
};

template<template<class...> class F, class L>
struct cl_mapcar_impl {
};

template<template<class...> class F, class... L> using cl_mapcar = typename cl_mapcar_impl<F, cl_list<L...>>::type;

template<template<class...> class F,
    template<class...> class L, class T>
struct cl_mapcar_impl<F, cl_list<L<T>>> {
  using type = L<F<T>>;
};

template<template<class...> class F,
    template<class...> class L, class... T, class U>
struct cl_mapcar_impl<F, cl_list<L<U, T...>>> {
  using type = cl_cons<F<U>, cl_mapcar<F, L<T...>>>;
};

template<template<class...> class F, class... R,
    template<class...> class L, class T>
struct cl_mapcar_impl<F, cl_list<L<T>, R...>> {
  using type = L<cl_apply<F, cl_cons<T, cl_mapcar<cl_car, cl_list<R...>>>>>;
};

template<template<class...> class F, class... L>
struct cl_mapcar_impl<F, cl_list<L...>> {
  using type = cl_cons<cl_apply<F, cl_mapcar<cl_car, cl_list<L...>>>, typename cl_mapcar_impl<F, cl_mapcar<cl_cdr, cl_list<L...>>>::type>;
};

template<class L>
struct cl_emptyp_impl {
};

template<template<class...> class L>
struct cl_emptyp_impl<L<>> : std::true_type {
};

template<template<class...> class L, class... T>
struct cl_emptyp_impl<L<T...>> : std::false_type {
};

template<class L> using cl_emptyp = typename cl_emptyp_impl<L>::type;

template<class... Args>
struct cl_append_impl {
  using type = cl_list<>;
};

template<class... L> using cl_append = typename cl_append_impl<L...>::type;

template<template<class...> class L, class... T>
struct cl_append_impl<L<T...>> {
  using type = L<T...>;
};

template<template<class...> class L, class... T, class... U>
struct cl_append_impl<L<T...>, L<U...>> {
  using type = L<T..., U...>;
};

template<class T, class U, class... L>
struct cl_append_impl<T, U, L...> {
  using type = cl_append<cl_append<T, U>, L...>;
};


template<class N, class L>
struct cl_nth_impl {
};

template<class N, class L> using cl_nth = typename cl_nth_impl<N, L>::type;

template<class L>
struct cl_nth_impl<std::integral_constant<std::size_t, 0>, L> {
  using type = cl_car<L>;
};

template<std::size_t N, class L>
struct cl_nth_impl<std::integral_constant<std::size_t, N>, L> {
  using type = cl_nth<std::integral_constant<std::size_t, N - 1>, cl_cdr<L>>;
};

template<template<class...> class F, class... L>
struct cl_apply_impl {
  using type = cl_apply<F, cl_append<cl_butlast<cl_list<L...>>, cl_apply<cl_list, cl_lastcar<cl_list<L...>>>>>;
};

template<template<class...> class F, class... T>
using cl_funcall = cl_apply<F, cl_list<T...>>;

template<class L>
struct cl_reverse_impl {
};

template<template<class...> class L, class... T>
struct cl_reverse_impl<L<T...>> {
  template<class A, class B>
  struct inner_impl {
  };

  template<class... A>
  struct inner_impl<L<>, L<A...>> {
    using type = L<A...>;
  };

  template<class A, class... B, class... C>
  struct inner_impl<L<A, B...>, L<C...>> {
    using type = typename inner_impl<L<B...>, L<A, C...>>::type;
  };

  using type = typename inner_impl<L<T...>, L<>>::type;
};

template<class L> using cl_reverse = typename cl_reverse_impl<L>::type;

template<class L>
struct list_integer_sequence_impl;

template<template<class...> class L, class... T>
struct list_integer_sequence_impl<L<T...>> {
  using type = std::integer_sequence<typename std::common_type<T...>::type, T::value...>;
};

template<class L> using list_integer_sequence = typename list_integer_sequence_impl<L>::type;

template<class S>
struct integer_sequence_array_impl;

template<class T, T... U>
struct integer_sequence_array_impl<std::integer_sequence<T, U...>> {
  static constexpr std::array<T, sizeof...(U)> value{U...};
};

template<class S> using integer_sequence_array = integer_sequence_array_impl<S>;

template<class L> using list_array = integer_sequence_array<list_integer_sequence<L>>;

#endif // CL_MPL_H
