# compiler
CC = g++

#Wall gives all warnings. 
CFLAGS = -Wall -g 

#these flags allow sdl libraries to compile
SDLFLAGS = -lSDL2 -L/usr/include/SDL2

#boost flags for statistical operations 
BSTFLAGS = -lboost_math_c99 -lboost_system

# defines what operations to do when making all
all: main

%.o: %.cpp Graph.h statisticalOperations.h NewEquationParser.h 3DGraph.h drawTriangle.h
	$(CC) $(CFLAGS) -c -o $@ $< $(SDLFLAGS) $(BSTFLAGS)
%.o: %.cpp Equation.h
	$(CC) $(CFLAGS) -c -o $@ $< $(SDLFLAGS)

# makes the main file with g++, flags amd target file of main.cpp. Outputs executable to main
main: main.o Graph.o Equation.o NewEquationParser.cpp 3DGraph.cpp
	$(CC) $(CFLAGS) -o $@ $^ $(SDLFLAGS)
# gets rid of main executable and all -o files
clean:
	rm -f main
	rm -f *.o

test: test.o Graph.o Equation.o NewEquationParser.cpp 3DGraph.cpp
	$(CC) $(CFLAGS) -o $@ $^ $(SDLFLAGS)
	

# all and clean are not compiling commands, just ones to help flow
.PHONY: all clean 
