#!/bin/bash

prefix="/"
destdir="/bitcoin/dist/isotopec-win64"

CONFIG_SITE="$PWD/depends/x86_64-w64-mingw32/share/config.site" \
    ./configure --prefix="${prefix}" --disable-tests
make -j4

x86_64-w64-mingw32-strip "src/isotopecd.exe"
x86_64-w64-mingw32-strip "src/isotopec-tx.exe"
x86_64-w64-mingw32-strip "src/qt/isotopec-qt.exe"
x86_64-w64-mingw32-strip "src/isotopec-cli.exe"
x86_64-w64-mingw32-strip "src/.libs/libisotopecconsensus-0.dll"

make install DESTDIR="${destdir}"
