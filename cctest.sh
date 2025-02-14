#!/bin/sh
mk () {
    make CC="$1" CFLAGS= > /dev/null
    bin/test
}

mk gcc
mk clang
mk tcc
mk musl-gcc
mk 'zig cc'
mk chibicc
mk lacc
rm *.o
