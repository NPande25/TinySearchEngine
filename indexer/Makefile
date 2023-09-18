# Nikhil Pande
# COSC 50, 23S
# Professor Palmer
#
# Makefile - indexer
#

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$(L) -I$(LL) 
CC = gcc
MAKE = make

# directories 
L = ../libcs50
LL = ../common

# programs
PROG = indexer
OBJS = indexer.o
TEST = indextest


# libraries
LIB = $(L)/libcs50.a
LIB1 = $(LL)/common.a

all: indexer indextest

$(PROG): $(OBJS) $(LIB1) $(LIB)
	$(CC) $(CFLAGS) $^ -lm -o $@


$(TEST): indextest.o $(LIB1) $(LIB)
	$(CC) $(CFLAGS) $^ -lm -o $@

indexer.o: indexer.c

indextest.o: indextest.c 

test: testing.sh
	bash -v ./testing.sh &> testing.out

clean:
	rm -f indexer.o
	rm -f indexer
	rm -rf index
	rm -f indextest.o
	rm -f indextest