#!/bin/sh
set -e

CFLAGS=-m32 CXXFLAGS=-m32 cmake ../ -DCMAKE_BUILD_TYPE=Release
make -j4
(cd ./bin-doom/; ./bin-doom)
