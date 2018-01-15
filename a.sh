#!/bin/bash
rm -rf Makefile.in Makefile configure
autoconf
automake --add-missing
./configure
make
