# compiler
CC = g++

#Wall gives all warnings. 
CFLAGS = -Wall -g

# defines what operations to do when making all
all: main

# makes the main file with g++, flags amd target file of main.cpp. Outputs executable to main
main: main.cpp
	$(CC) $(CFLAGS) -o $@ $^

# gets rid of main executable and all -o files
clean:
	rm -f main
	rm -f *.o

# all and clean are not compiling commands, just ones to help flow
.PHONY: all clean 
