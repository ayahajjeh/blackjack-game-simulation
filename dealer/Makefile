# Makefile for 'dealer' module
# 
# adapted from Makefile in bag
#
# Daniel Lampert
# CS 50, Fall 2022

OBJS = dealer.o ../cards/cards.o ../network/network.o
LIBS =

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../lib
CC = gcc
MAKE = make
# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

.PHONY: all test clean

all: dealer

dealer: $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

dealer.o: ../network/network.h ../cards/cards.h

test: dealer
	bash -v testing.sh

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f dealer
	rm -f core