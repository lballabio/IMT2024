
.PHONY: all build test

all: build test

build: main

test: main
	./main

main: *.hpp *.cpp
	g++ -I/opt/homebrew/opt/boost/include *.cpp `quantlib-config --cflags` -g0 -O3 `quantlib-config --libs` -o main

