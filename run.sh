#!/bin/sh

set -e 

cd build
ninja
cd ..
build/seven