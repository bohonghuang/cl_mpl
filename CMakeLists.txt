cmake_minimum_required(VERSION 3.25)

project(cl_mpl)

set(CMAKE_CXX_STANDARD 20)

add_executable(cl_mpl_test main.cpp cl_mpl.h)
add_executable(cl_mpl_fib fib.cpp cl_mpl.h)
