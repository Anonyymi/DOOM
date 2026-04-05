#!/bin/bash
set -ex

if [ "$1" != "64" ] && [ "$1" != "32" ]; then
    echo "error: arg #1 must be '32|64'"
    exit 1
fi

if [ "$2" != "Release" ] && [ "$2" != "Debug" ]; then
    echo "error: arg #2 must be 'Release|Debug'"
    exit 1
fi

BUILD_ARCH="$1"
BUILD_TYPE="$2"
BUILD_DIR="./build/"

mkdir -p $BUILD_DIR
pushd $BUILD_DIR

CFLAGS=-m${BUILD_ARCH} CXXFLAGS=-m${BUILD_ARCH} cmake ../ -DCMAKE_BUILD_TYPE="${BUILD_TYPE}"
make -j
(cd ./bin-doom/; ./bin-doom)
