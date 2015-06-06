#!/bin/bash
# Simple build script to compile project

cd ${PWD##}/utils
make clean
make

cd ..
make clean
make

echo "[-] Build done"