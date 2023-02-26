#include <iostream>

#include "cl_mpl.h"

// (defun fib-seq (n)
//     (labels ((inner (seq i)
//                (cond
//                  ((<= i 2) (inner (cons 1 seq) (1+ i)))
//                  ((<= i n) (inner (reverse (cons (+ (nth (- i 1) seq) (nth (- i 2) seq)) (reverse seq))) (1+ i)))
//                  (t seq))))
//       (inner nil 0)))

template<size_t N>
struct fib_seq_impl {
  template<class Seq, size_t I, class = void>
  struct inner_impl {
    using type = Seq;
  };

  template<class Seq, size_t I>
  struct inner_impl<Seq, I, std::enable_if_t<I <= 2>> {
    using type = typename inner_impl<cl_cons<std::integral_constant<size_t, 1>, Seq>, I + 1>::type;
  };

  template<class Seq, size_t I>
  struct inner_impl<Seq, I, std::enable_if_t<2 < I && I <= N>> {
    using type = typename inner_impl<cl_reverse<cl_cons<std::integral_constant<size_t,
        cl_nth<std::integral_constant<size_t, I - 1>, Seq>::value +
        cl_nth<std::integral_constant<size_t, I - 2>, Seq>::value>, cl_reverse<Seq>>>, I + 1>::type;
  };

  using type = list_array<typename inner_impl<cl_list<>, 0>::type>;
};

template<size_t N>
constexpr std::array<size_t, N + 1> fib_seq() {
  return fib_seq_impl<N>::type::value;
}

template<size_t N>
constexpr size_t fib() {
  return fib_seq<N>()[N];
}

int main(int argc, char *argv[]) {
  for (auto i: fib_seq<39>()) {
    std::cout << i << std::endl;
  }
  std::cout << fib<40>() << std::endl;
  return 0;
}
