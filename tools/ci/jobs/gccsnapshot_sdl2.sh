#!/bin/bash

export CC=gcc
export CXX=g++
export LOGFILE=gcc-snapshot.log
export PATH=/usr/lib/gcc-snapshot/bin:$PATH

source ./tools/ci/scripts/init.sh

export CXXFLAGS="-pedantic -ggdb3 -O2 -pipe -Wstrict-aliasing=2 \
-Wstrict-overflow=1 -Wformat=1 -D_FORTIFY_SOURCE=2 \
-std=gnu++1z -Wformat=1 \
-Wno-attributes"

source ./tools/ci/flags/gccsnapshot.sh

do_init
run_configure --enable-werror --with-sdl2
run_make

source ./tools/ci/scripts/exit.sh

exit 0
