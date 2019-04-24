cmake_minimum_required(VERSION 3.11)
project(wszst_yaz0_c CXX)

find_package(pybind11 REQUIRED)
pybind11_add_module(wszst_yaz0_c
  main.cpp
)
add_library(wszst_yaz0_c::pybind11_binding ALIAS wszst_yaz0_c)

target_compile_options(wszst_yaz0_c PRIVATE -Wall -Wextra)
set_target_properties(wszst_yaz0_c PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)
