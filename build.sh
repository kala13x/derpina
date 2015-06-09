#!/bin/bash
# Simple build script to compile project

cd ${PWD##}/slog
make clean
make

cd ../sms
make clean
make

cd ../src
make clean
make

cd ..

echo "[-] Build done"
