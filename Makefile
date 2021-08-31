
CXX			= g++
CXXFLAGS	= -std=c++17 -O2 -Wall

ODIR		= ./build
BDIR		= ./bin
IDIR		= ./brainfuck

bfmachine: dir
	$(CXX) -o $(BDIR)/$@ $(IDIR)/bfmachine.cpp $(IDIR)/bfmake.cpp $(CXXFLAGS)

dir: $(IDIR)/bfmachine.cpp $(IDIR)/bfmachine.hpp $(IDIR)/bfmake.cpp
	mkdir -p bin build

clean:
	rm -r bin