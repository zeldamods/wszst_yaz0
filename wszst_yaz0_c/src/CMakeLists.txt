cmake_minimum_required(VERSION 3.10)
project(wszst_yaz0_c CXX)

if (MSVC)
  set(PYBIND11_CPP_STANDARD /std:c++latest)
else()
  set(PYBIND11_CPP_STANDARD -std=c++17)
endif()

find_package(pybind11 REQUIRED)
pybind11_add_module(wszst_yaz0_c
  main.cpp
)
add_library(wszst_yaz0_c::pybind11_binding ALIAS wszst_yaz0_c)

if (MSVC)
  target_compile_options(wszst_yaz0_c PRIVATE /W4)
else()
  target_compile_options(wszst_yaz0_c PRIVATE -Wall -Wextra)
endif()
