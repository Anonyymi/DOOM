#!/bin/sh
set -e

CFLAGS=-m64 CXXFLAGS=-m64 cmake ../
make -j4
(cd ./bin-doom/; ./bin-doom)
