#!/bin/bash

if [[ $1 == 'generate' ]]
then
    export CC=/usr/bin/gcc-10
    export CXX=/usr/bin/g++-10
    cmake -S . -B build -G"Unix Makefiles"
elif [[ $1 == 'build' ]]
then
    build="all"
    if [[ "$2" != "" ]]
    then
        build=$2
    fi
    cmake --build build --target all -j8
elif [[ $1 == 'run' ]]
then 
    ./build/src/draughts --black=random --white=human
else
    echo 'Invalid or no argument'
fi
