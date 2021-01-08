#!/bin/bash

if [[ $1 == 'generate' ]]
then
    export CC=/usr/bin/gcc-10
    export CXX=/usr/bin/g++-10
    cmake -S . -B build -G"Unix Makefiles"
elif [[ $1 == 'build' ]]
then
    target="all"
    if [[ "$2" != "" ]]
    then
        target=$2
    fi
    cmake --build build --target $target -j8
elif [[ $1 == 'run' ]]
then 
    ./build/src/draughts --black=minmax-7 --white=minmax-6
elif [[ $1 == 'debug' ]]
then 
    gdb --args ./build/src/draughts --black=minmax-3 --white=minmax-3
elif [[ $1 == 'type' ]]
then
    if [[ $2 != 'release' ]] && [[ $2 != 'debug' ]]
    then
        echo 'Invalid build type: $2'
    else
        cmake build/ -DCMAKE_BUILD_TYPE=$2
    fi
else
    echo 'Invalid or no argument'
fi
