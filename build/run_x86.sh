#!/bin/sh
set -e

CFLAGS=-m32 CXXFLAGS=-m32 cmake ../
make -j4
(cd ./bin-doom/; ./bin-doom)
