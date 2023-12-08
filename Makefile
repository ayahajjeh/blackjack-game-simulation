# Makefile for CS50 Blackjack Final Project
#
# David Kotz - April 2016, 2017, 2021
# CS 50, Fall 2022

.PHONY: all clean

############## default: make all libs and programs ##########
# If libcs50 contains set.c, we build a fresh libcs50.a;
# otherwise we use the pre-built library provided by instructor.
all:
	make -C cards
	make -C network
	make -C dealer
	make -C player

############### TAGS for emacs users ##########
TAGS:  Makefile */Makefile */*.c */*.h */*.md */*.sh
	etags $^

############## clean  ##########
clean:
	rm -f *~
	rm -f TAGS
	make -C cards clean
	make -C network clean
	make -C dealer clean
	make -C player clean