CXX = g++
CXXFLAGS = -g -Wall -std=c++11

all: main

main: main.o bst.h avlbst.h print_bst.h
	$(CXX) $(CXXFLAGS) main.o -o $@
	
clean:
	rm *.o