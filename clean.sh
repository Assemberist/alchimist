#!/bin/bash

cd submod/string_tree
make clean
cd -

cd archivator
make clean
cd -

cd core
make clean
cd -

rm	alchimist \
    library.alch2 \
    parser

rm -r saves

