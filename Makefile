CC=g++
FLAGS=-g -O2 -Wall -Werror -pedantic -std=c++11
FILE = model
LIBRARIES = src/get_params.cpp

all:
	$(CC) $(FLAGS) $(FILE).cpp $(LIBRARIES) -o $(FILE) -lsimlib -lm