cmake_minimum_required(VERSION 3.0)

project(lepy)

set(CMAKE_CXX_STANDARD 14)

if (DEFINED ENV{PYBIND11_DIR})
   set(PYBIND11_DIR $ENV{PYBIND11_DIR})
else()
   message(FATAL_ERROR "Set PYBIND11_DIR variable")
endif()

add_subdirectory(${PYBIND11_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/pybind11)

pybind11_add_module(${PROJECT_NAME} py_interface.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE light-engine)

if (DEFINED VERSION)
    add_definitions(-DVERSION=\"${VERSION}\")
else()
    add_definitions(-DVERSION=\"Custom build\")
endif()

 add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
      COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/setup.py --version ${VERSION} --bin-path ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} --source-path ${CMAKE_CURRENT_SOURCE_DIR}
      WORKING_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
      COMMENT "Generate python whl file..."
      )

install(DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/dist DESTINATION python PATTERN "*.whl")
