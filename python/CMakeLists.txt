cmake_minimum_required(VERSION 3.0)

project(pyle)

if (DEFINED ENV{PYBIND11_DIR})
   set(PYBIND11_DIR $ENV{PYBIND11_DIR})
else()
   message(FATAL_ERROR "Set PYBIND11_DIR variable")
endif()

add_subdirectory(${PYBIND11_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/pybind11)

pybind11_add_module(${PROJECT_NAME} py_interface.cpp)
