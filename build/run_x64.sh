#!/bin/sh
set -e

CFLAGS=-m64 CXXFLAGS=-m64 cmake ../ -DCMAKE_BUILD_TYPE=Release
make -j4
(cd ./bin-doom/; ./bin-doom)
