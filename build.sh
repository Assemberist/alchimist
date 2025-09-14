#!/bin/bash

if_copy(){
    if [ $1 -nt $(basename $1) ];
    then
        cp $1 $(basename $1)
    fi
}

build_dir(){
    cd $1 && make
    cd -
}

if [ -f submod/string_tree/Makefile ];
then
    git submodule init
    git submodule update
fi

build_dir submod/string_tree

build_dir archivator
if_copy archivator/parser

build_dir core
if_copy core/alchimist

./parser archivator/for_test