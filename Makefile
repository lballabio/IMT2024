# We had to retrieve the quantlib headers directory in order for the code to work
# even when we were not in the same folder as where Quantlib was stored 
QL_INCLUDE := /path/to/quantlib/headers
QL_LIBS := -L/path/to/quantlib/lib -lQuantLib


.PHONY: all build test

all: build test

build: main

test: main
	./main

main: *.hpp *.cpp
	g++ *.cpp -I$(QL_INCLUDE) -g0 -O3 $(QL_LIBS) -o main
