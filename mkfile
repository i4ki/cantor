ALL=build test

DEPS=\
	src/bitset

all:V: $ALL

build:V: 
	./build
	./test
	echo "Build successfully"

