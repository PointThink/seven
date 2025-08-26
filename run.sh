#!/bin/sh

set -e 

cd build
ninja
cd ..
build/seven edit level.txt
