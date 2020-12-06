CC=g++
FLAGS=-g -O2 -Wall -Werror -pedantic -std=c++11
FILE = covid19
LIBRARIES = src/get_params.cpp

all:
	$(CC) $(FLAGS) $(FILE).cpp $(LIBRARIES) -o $(FILE) -lsimlib -lm

run0: 
	./${FILE} -v -t 0 -o covid19-s0.csv
run1:
	./${FILE} -v -t 1 -x 0.1 -q 0.6 -o covid19-s1.csv
run2:
	./${FILE} -v -t 2 -x 0.1 -k 0.5 -q 0.6 -o covid19-s2.csv
run3:
	./${FILE} -v -t 3 -x 0.1 -k 0.25 -q 0.6 -o covid19-s3.csv

runcz-pes3:
	./${FILE} -v -t 4 -r 200 -S 10680000 -I 4917 -R 19783 -D 428 -c 29 -h 74760 -f 0.017 -1 0.05 -2 0.01 -3 0.01 -4 0.01 -5 0.03 -6 0.15 -7 0.2 -8 0.25 -9 -0.35 -o covid19-cz

runcz-pes4:
	./${FILE} -v -t 4 -r 200 -S 10680000 -I 4917 -R 19783 -D 428 -c 29 -h 74760 -f 0.017 -1 0.05 -2 0.01 -3 0.01 -4 0.01 -5 0.03 -6 0.15 -7 0.2 -8 0.25 -9 -0.0 -o covid19-cz.csv