
cmake_minimum_required( VERSION 2.6 )

project( IsItFast CXX )

include_directories(  $ENV{TEMPLATIOUS_DIR} )

file( GLOB bench_files
    "benchmarks/*.cpp" )

add_executable( a.out main.cpp BenchRunner.cpp
    ${bench_files} )

ADD_DEFINITIONS(
    -std=c++11
    -lstdc++
    -Wall
    -x c++
    -g
    -O3
)

SET( SET_CMAKE_CXX_COMPILER "g++-4.8" )

SET( CMAKE_EXPORT_COMPILE_COMMANDS ON )
