# Nikhil Pande
# COSC 50, 23S
# Professor Palmer
#
# Makefile - crawler
#

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I$(L) -I$(LL) 
CC = gcc
MAKE = make


# directories 
L = ../libcs50
LL = ../common

# programs
PROG = crawler
OBJS = crawler.o

# libraries
LIB = $(L)/libcs50.a
LIB1 = $(LL)/common.a

# URL for valgrind
URL = http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html

$(PROG): $(OBJS) $(LIB) $(LIB1)
	$(CC) $(CFLAGS) $^ -lm -o $@

crawler.o: crawler.c 

test: testing.sh
	bash -v ./testing.sh &> testing.out

valgrind: crawler.c
# create directory if not already there
	$(shell mkdir -p data/valgrind)
	valgrind ./crawler $(URL) data/valgrind 

clean:
	rm -f crawler
	rm -f crawler.o
	rm -rf data
