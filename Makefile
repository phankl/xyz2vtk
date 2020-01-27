#Makefile to compile the xyz2vtk tool

CXX = g++
CXXFLAGS = -O2 -g -std=c++11

MAIN = xyz2vtk

SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)

$(MAIN): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -f $(OBJ) $(MAIN)
