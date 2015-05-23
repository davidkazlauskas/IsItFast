# IsItFast
Benchmarks for templatious library to test performance for specific compilers and processors.
[![Build Status](https://travis-ci.org/davidkazlauskas/IsItFast.svg)](https://travis-ci.org/davidkazlauskas/IsItFast)

Compile and run this project to find out how good templatious library will perform on your compiler. TEMPLATIOUS_DIR enviroment variable should be defined.

~~~~~~
export TEMPLATIOUS_DIR=<path to templatious library>
git clone https://github.com/davidkazlauskas/IsItFast.git
cd IsItFast
mkdir build
cd build
cmake ..
make
./a.out

# view results
firefox view.html
~~~~~~

To use specific compiler (for instance, clang++-3.5)
cmake step should be like:
~~~~~~
cmake -DCMAKE_CXX_COMPILER=clang++-3.5 ..
~~~~~~

To find code chunks that represent charts in browser you can grep titles.
For instance, to find
~~~~~~~
- Dynamic templatious virtual match functor. [templatious_dynamic_vpack]
~~~~~~~

One could execute the following command in this directory:
~~~~~~~
grep -r "templatious_dynamic_vpack" .
~~~~~~~

