#!/bin/bash

sudo true

rm -R build

cmake -H. -Bbuild

cmake --build build -- -j3

ln -sf build/compile_commands.json compile_commands.json

sudo setcap cap_net_raw+ep bin/lhf
