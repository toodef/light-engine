#!/bin/sh

mkdir build
cd build

git clone https://github.com/pybind/pybind11.git
export PYBIND11_DIR=$PWD/pybind11

cmake -DCMAKE_CXX_FLAGS=-fPIC -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX:PATH=installed_lib -DBUILD_EXAMPLES="OFF" -DBUILD_PYTHON="ON" -DVERSION="0.0.2" ..
make

cd ..
