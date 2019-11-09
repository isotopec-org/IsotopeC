#!/bin/bash

prefix="/"
destdir="/bitcoin/dist/isotopec-win32"

CONFIG_SITE="$PWD/depends/i686-w64-mingw32/share/config.site" \
    ./configure --prefix="${prefix}" --disable-tests
make -j4

i686-w64-mingw32-strip "src/isotopecd.exe"
i686-w64-mingw32-strip "src/isotopec-tx.exe"
i686-w64-mingw32-strip "src/qt/isotopec-qt.exe"
i686-w64-mingw32-strip "src/isotopec-cli.exe"
i686-w64-mingw32-strip "src/.libs/libisotopecconsensus-0.dll"

make install DESTDIR="${destdir}"
