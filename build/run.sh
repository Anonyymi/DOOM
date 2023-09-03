#!/bin/sh
set -e

make -j4
(cd ./bin-doom/; ./bin-doom)
