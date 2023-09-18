# Nikhil Pande
# COSC 50, 23S
# Professor Palmer
#
# Makefile - common library
#

OBJS = pagedir.o index.o word.o query.o readlinep.o ../libcs50/libcs50.a
LIB = common.a
LIB2 = ../libcs50

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50
CC = gcc
MAKE = make

.PHONY: clean

$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

pagedir.o: pagedir.h

index.o: index.h $(LIB2)/set.h $(LIB2)/counters.h $(LIB2)/hashtable.h $(LIB2)/file.h

word.o: word.h

query.o: query.h $(LIB2)/counters.h

readlinep.o: readlinep.h

clean:
	rm -f core
	rm -f $(LIB) *~ *.o

