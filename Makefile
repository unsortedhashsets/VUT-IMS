CC=g++
FLAGS=-g -O2 -Wall -Werror -pedantic -std=c++11
FILE = covid19
LIBRARIES = src/get_params.cpp

all:
	$(CC) $(FLAGS) $(FILE).cpp $(LIBRARIES) -o $(FILE) -lsimlib -lm

run0: 
	./${FILE} -v -t 0 
run1:
	./${FILE} -v -t 1 -i 0.1 -q 0.6
run2:
	./${FILE} -v -t 2 -i 0.1 -k 0.5 -q 0.6
run3:
	./${FILE} -v -t 3 -i 0.1 -k 0.25 -q 0.6
