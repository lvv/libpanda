#!/bin/sh

aclocal
libtoolize --force --copy
autoheader
autoconf
automake --add-missing --copy
./configure $@
