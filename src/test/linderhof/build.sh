#!/bin/bash

cmake -H. -Bbuild

cmake --build build -- -j3

ln -sf build/compile_commands.json compile_commands.json
