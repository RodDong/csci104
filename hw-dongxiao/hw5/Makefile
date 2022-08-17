CXX = g++
CXXFLAGS = -g -Wall -std=c++11

all: gridpaths sat_solver

gridpaths: gridpaths.o
	$(CXX) $(CXXFLAGS) gridpaths.o -o $@
	
sat_solver: sat_solver.o
	$(CXX) $(CXXFLAGS) sat_solver.o -o $@

sat_solver.o: sat_solver.cpp avlbst.h
	$(CXX) $(CXXFLAGS) -c $(basename $@).cpp -o $@

clean:
	rm *.o