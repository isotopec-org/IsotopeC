#!/bin/bash

prefix="/usr/local"
version="1.0.3"
destdir="/bitcoin/dist/isotopec-${version}-linux"

CONFIG_SITE="$PWD/depends/x86_64-pc-linux-gnu/share/config.site" \
    ./configure --prefix="${prefix}" --disable-tests
make clean && make -j4

strip "src/isotopecd"
strip "src/isotopec-tx"
strip "src/qt/isotopec-qt"
strip "src/isotopec-cli"
strip "src/.libs/libisotopecconsensus.so"

make install DESTDIR="${destdir}"
