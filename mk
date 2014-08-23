#!/bin/bash

# This is all auxillary functions to simplify the build script.
# I hate makefiles, and cmake so shell scripts work.

CXX="g++"
CXXFLAGS="$CXXFLAGS -fPIC -std=gnu++11 -Wall -Werror -Wno-error=pointer-arith"
LD="g++"
LDFLAGS="$LDFLAGS -fPIC -L$LIB $LDFLAGS -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf"
AR="ar rcs"
GEN="/bin/bash ./gen.sh"

CXX_CMD="$CXX $CXXFLAGS"
LD_CMD="$LD $LDFLAGS"
CA_CMD="$CXX $CXXFLAGS $LDFLAGS"

FILES=$2

function mkcc_ {
	echo "CXX 		$2"
	$CXX_CMD $INCLUDE -c $1 -o $2
}

function mkld_ {
	echo "LD 		$2"
	$LD_CMD $1 -o $2
}

function mkgen_ {
	echo "GEN		$1"
	bash -c "cd ../$1 && $GEN"
}

function mkcc {
	mkcc_ $SRC/$1/$2.cpp $2.cpp.o
}

function mkmcc {
	for f in `ls $SRC/$1/*.cpp`; do
		mkcc_ $f $(basename $f).o
	done
}

function mkld {
	echo "LD 		$1"
	$LD_CMD ./*.o $LIB/*.a -o $BIN/$1
}

function mkar {
	echo "AR 		$1"
	$AR $LIB/$1.a ./*.o
}

function newtd {
	mkdir td
	cd td
}

function deltd {
	cd ..
	rm -rf td
}
