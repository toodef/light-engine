#!/bin/sh

mkdir build
cd build

cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX:PATH=installed_lib -DBUILD_EXAMPLES="OFF" -DBUILD_PYTHON="OFF" -DVERSION="0.0.2" ..
make

cd ..
