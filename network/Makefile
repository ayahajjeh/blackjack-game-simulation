# Makefile for 'network' module
# 
# adapted from Makefile in bag
#
# Daniel Lampert
# CS 50, Fall 2022

OBJS = network.o
LIBS =

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../lib
CC = gcc
MAKE = make
# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

.PHONY: all clean

all: serverTest clientTest

serverTest: $(OBJS) serverTest.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

clientTest: $(OBJS) clientTest.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

serverTest.o: network.h
clientTest.o: network.h
network.o: network.h

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f clientTest
	rm -f serverTest
	rm -f core
