#!/bin/sh
cmake -S . -B Architecture-Project-debug -DCMAKE_BUILD_TYPE=Debug
cmake -S . -B Architecture-Project-release -DCMAKE_BUILD_TYPE=Release
cmake --build Architecture-Project-debug
cmake --build Architecture-Project-release