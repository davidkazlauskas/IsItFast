
cmake_minimum_required( VERSION 2.6 )

project( IsItFast CXX )

include_directories(  $ENV{TEMPLATIOUS_DIR} )

file( GLOB bench_files
    "benchmarks/*.cpp" )

set ( FLAGS
    -std=c++11
    -lstdc++
    -Wall
    -x c++
    -g
    -O3
)

file( WRITE ${CMAKE_BINARY_DIR}/compiler-info.json
    "
    {
        \"flags\": \"${FLAGS}\",
        \"compiler\": \"${CMAKE_CXX_COMPILER}\"
    }
    "
)

add_executable( a.out main.cpp BenchRunner.cpp
    ${bench_files} )

SET( DISPLAY_HTML view.html drawgraph.js )
SET( JAVASCRIPT_LIBS jquery-1.9.0.min.js Chart.min.js )

file( COPY ${DISPLAY_HTML} ${JAVASCRIPT_LIBS} DESTINATION ${CMAKE_BINARY_DIR} )

ADD_DEFINITIONS( ${FLAGS} )

#SET( CMAKE_CXX_COMPILER "g++-4.8" )
SET( CMAKE_CXX_COMPILER "clang++-3.5" )

SET( CMAKE_EXPORT_COMPILE_COMMANDS ON )
