#!/bin/bash

# This is all auxillary functions to simplify the build script.
# I hate makefiles, and cmake so shell scripts work.

CXX="g++"
CXXFLAGS="$CXXFLAGS -std=gnu++11 -Wall -Werror -Wno-error=pointer-arith"
LD="g++"
LDFLAGS="$LDFLAGS -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf"
AR="ar rcs"
GEN="/bin/bash ./gen.sh"

CA_CMD="$CXX $CXXFLAGS $LDFLAGS"

FILES=$2

function mkcc_ {
	echo "CXX 		$2"
	$CXX $CXXFLAGS $INCLUDE -c $1 -o $2
}

function mkld_ {
	echo "LD 		$2"
	$LD $LDFLAGS $CXXFLAGS $1 -o $2
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
	$LD $CXXFLAGS ./*.o $LDFLAGS -o $BIN/$1
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
