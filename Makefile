CC=g++
FLAGS=-g -O2 -Wall -Werror -pedantic -std=c++11
FILE = covid19
LIBRARIES = src/get_params.cpp

all:
	$(CC) $(FLAGS) $(FILE).cpp $(LIBRARIES) -o $(FILE) -lsimlib -lm