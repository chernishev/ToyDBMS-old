CC=g++
#CXXFLAGS=-std=c++11
CXXFLAGS=-std=c++11 -g -O0
SRC=../interface/interface.cpp pgetnextnode.cpp pselectnode.cpp pjoinnode.cpp demo.cpp
HDR=../interface/interface.h ../interface/basics.h pgetnextnode.h pjoinnode.h pselectnode.h
OBJ=../interface/interface.o pgetnextnode.o pselectnode.o pjoinnode.o demo.o

all: demo

valgrind: demo
	valgrind --leak-check=full --track-origins=yes ./demo

restyle-all:
	astyle --suffix=none --options=../../style.astylerc $(SRC) $(HDR)

demo: $(OBJ)
	$(CC) $(OBJ) $(CXXFLAGS) -o $@

%.o: %.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f demo
	rm -f *.o
	rm -f ../interface/*.o