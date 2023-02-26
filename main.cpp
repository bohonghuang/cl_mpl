#include <iostream>

#include "cl_mpl.h"

int main(int argc, char *argv[]) {
  cl_mapcar<std::tuple,
      std::tuple<int, int, int>,
      std::tuple<float, float, float>,
      std::tuple<bool, bool, bool>> tuple3_1;

  std::tuple<std::tuple<int, float, bool>,
      std::tuple<int, float, bool>,
      std::tuple<int, float, bool>> tuple3_2;

  static_assert(std::is_same<decltype(tuple3_1), decltype(tuple3_2)>());

  static_assert(
      std::is_same<std::tuple<std::add_pointer<int>, std::add_pointer<int>, std::add_pointer<int>>, cl_mapcar<std::add_pointer, std::tuple<int, int, int>>>());
  static_assert(
      std::is_same<std::tuple<std::pair<int, float>, std::pair<float, char>, std::pair<char, int>>, cl_mapcar<std::pair, std::tuple<int, float, char>, std::tuple<float, char, int>>>());

  static_assert(
      std::is_same<std::tuple<int, int, float, float>, cl_append<std::tuple<int, int>, std::tuple<float, float>>>());

  static_assert(
      std::is_same<std::uint32_t, cl_nth<std::integral_constant<size_t, 2>, std::tuple<std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t>>>());

  static_assert(std::is_same<std::tuple<int, char>, cl_butlast<std::tuple<int, char, float>>>());

  static_assert(
      std::is_same<std::tuple<int, float, char, short>, cl_apply<std::tuple, int, float, std::tuple<char, short>>>());

  static_assert(std::is_same<std::tuple<int, float>, cl_funcall<std::tuple, int, float>>());

  static_assert(std::is_same<std::tuple<int, float, char>, cl_reverse<std::tuple<char, float, int>>>());
}
