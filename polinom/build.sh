#!/bin/bash

# Stop on error
set -e

# Don't test the speed with this, because debug builds are slow
g++ main.cpp -std=c++11 -g -Wall -o speed_test_float_debug -DFLOAT_TYPE_OVERRIDE=float
g++ main.cpp -std=c++11 -g -Wall -o speed_test_double_debug -DFLOAT_TYPE_OVERRIDE=double
g++ main.cpp -std=c++11 -g -Wall -o speed_test_long_double_debug -DFLOAT_TYPE_OVERRIDE="long double"

# -O3: optimize fully
# -s: strip all debug symbols
# -save-temps: print out generated assembly
g++ main.cpp -std=c++11 -O3 -s -Wall -save-temps -o speed_test_float -DFLOAT_TYPE_OVERRIDE=float
g++ main.cpp -std=c++11 -O3 -s -Wall -save-temps -o speed_test_double -DFLOAT_TYPE_OVERRIDE=double
g++ main.cpp -std=c++11 -O3 -s -Wall -save-temps -o speed_test_long_double -DFLOAT_TYPE_OVERRIDE="long double"

echo float
./speed_test_float

echo double
./speed_test_double

echo long double
./speed_test_long_double

