# Nikhil Pande
# COSC 50, 23S
# Tiny Search Engine
#
# querier.c makefile
#

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50 -I../common
LIBS = ../common/common.a ../libcs50/libcs50.a
OBJS = querier.o

all: querier

querier: $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -lm -o querier

querier.o: querier.c $(LIBS)

clean:
	rm -rf *.x *.o
	rm -rf vgcore*
	rm -rf querier