# Makefile for 'player' module
#
# Barkin Cavdaroglu
# CS 50, Fall 2022

OBJS = entry_point.o training.o player.o qtable.o ../cards/cards.o ../network/network.o #../dealer/dealer-given.o
LIBS =

# uncomment the following to turn on verbose memory logging
TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../dealer -I../network -I../cards
CC = gcc
MAKE = make
# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all -s

entry: $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

player: $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

training: $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

test: ../dealer/dealer player
	bash -v testing.sh

entry_point.o: ../cards/cards.h ../network/network.h
player.o: player.h ../cards/cards.h ../network/network.h
training.o: training.h ../network/network.h ../cards/cards.h 
qtable.o: qtable.h
../cards/cards.o: ../cards/cards.h
../network/network.o: ../network/network.h

.PHONY: test valgrind clean

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f training
	rm -f core
	rm -f entry