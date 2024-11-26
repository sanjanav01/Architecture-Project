#!/bin/sh
cmake -S . -B project-debug -DCMAKE_BUILD_TYPE=Debug
cmake -S . -B project-release -DCMAKE_BUILD_TYPE=Release
cmake --build project-debug
cmake --build project-release