CXX=g++
CXXFLAGS=-std=c++11 -Wall -Werror
IFLAGS=-I ${BOOST_ROOT}
OPTFLAGS=-O3 -DNDEBUG
OPT=opt
DBGFLAGS=-g
DBG=dbg

clean:
	rm *-opt *-dbg

