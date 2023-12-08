# Makefile for cards
#
# Elaine Pu. 2022
# CS 50, Fall 2022

OBJS = cardsTest.o cards.o  
LIBS =

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) 
CC = gcc
MAKE = make
MAKECLEAN = make clean
# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

cardsTest: $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

.PHONY: test valgrind clean

test: cardsTest
	./cardsTest >& testing.out

valgrind: cardsTest 
	$(VALGRIND) ./cardsTest

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f cardsTest
	rm -f core

