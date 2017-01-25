#!/bin/bash

export CC=gcc-6
export CXX=g++-6
export LOGFILE=gcc6.log

source ./tools/ci/scripts/init.sh

export CXXFLAGS="-pedantic -ggdb3 -O2 -pipe -Wstrict-aliasing=2 \
-Wstrict-overflow=1 -Wformat=1 -D_FORTIFY_SOURCE=2 \
-std=gnu++1z -Wformat=1 \
-Wno-attributes"

source ./tools/ci/flags/gcc6.sh

do_init
run_configure --enable-werror $*
run_make

source ./tools/ci/scripts/exit.sh

exit 0
