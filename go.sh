#!/bin/bash
clang++ -g -fsanitize=address -fsanitize-coverage=trace-pc-guard -std=c++11 -Wno-format-security -Wno-unused-result serv_harness.cpp libFuzzer.a
g++ -ggdb -std=c++11 serv_harness.cpp -Wno-format-security -Wno-unused-result -o a.dbg

#run fuzzing
./a.out
