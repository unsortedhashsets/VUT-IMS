#################################################################
# Makefile for SIMLIB (unix)

MAKEFILE = Makefile.`uname -s`-`uname -m`
MAKEFILE32 = Makefile.`uname -s`-i686
MAKEFILE64 = Makefile.`uname -s`-x86_64

#debug: 
#	make -f Makefile.Linux-i686
OBJECT = ball2.cc

#################################################################
all: 
	g++ -g -O2 -o model $(OBJECT) -lsimlib -lm