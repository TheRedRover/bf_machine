
CXX			= g++
CXXFLAGS	= -std=c++17 -O2 -Wall

ODIR		= ./build
BDIR		= ./bin
IDIR		= ./brainfuck

bfmachine: dir
	$(CXX) -o $(BDIR)/$@ $(IDIR)/bfmachine.cpp $(IDIR)/bfmake.cpp $(CXXFLAGS)

bfmachine.o: $(IDIR)/bfmachine.cpp bfmachine dir
	$(CXX) $(CXXFLAGS) -c $< -o $(ODIR)/$@

test/test: test/test.cpp bfmachine.o $(ODIR)/bfmachine.o
	$(CXX) $(CXXFLAGS) test/test.cpp $(ODIR)/bfmachine.o -o $@
#	sh /test/test || (echo "test failed $$?"; exit 1)

test: test/test
	echo "all done"

dir: $(IDIR)/bfmachine.cpp $(IDIR)/bfmachine.hpp $(IDIR)/bfmake.cpp
	mkdir -p bin build

all:test/test bfmachine

clean:
	rm -r $(BDIR)
	rm test/test
	rm -r $(ODIR)