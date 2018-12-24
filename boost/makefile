CXX=g++
CXXFLAGS=-std=c++11 -Wall -Werror
IFLAGS=-I ${BOOST_ROOT}
OPT=-O3 -DNDEBUG
DEBUG=-g


smart-pointers:
	$(CXX) $(CXXFLAGS) $(IFLAGS) $(OPT) smart_pointers.cpp -o smart_pointers-opt
	$(CXX) $(CXXFLAGS) $(IFLAGS) $(DEBUG) smart_pointers.cpp -o smart_pointers-dbg


clean:
	rm *-opt *-dbg

