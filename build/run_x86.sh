#!/bin/sh
set -e

CFLAGS=-m32 CXXFLAGS=-m32 -DCMAKE_BUILD_TYPE=Release cmake ../
make -j4
(cd ./bin-doom/; ./bin-doom)
