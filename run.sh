#!/bin/bash

make clean
make
rm -rf test_data
mkdir test_data
./bin/generator
rm -rf ../fast_assembler/dbg
mv test_data ../fast_assembler/dbg
