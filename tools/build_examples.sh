#!/bin/sh

mkdir build
cd build

cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX:PATH=installed_lib -DBUILD_EXAMPLES="ON" -DBUILD_PYTHON="OFF" -DVERSION="0.0.2" ..
make all

cd ..