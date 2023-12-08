#!/bin/bash
#
# testing.sh - testing program for dealer.c
#
# input: none
# output: prints results of testing different inputs for dealer.c to testing.out
#
# usage: bash -v testing.sh >& testing.out OR make test >& testing.out
#
# Daniel Lampert, CS 50, Fall 2022

#myvalgrind = valgrind --leak-check=full --show-leak-kinds=all

###### incorrect #s of arguments ######
# zero arguments                                                                    
./dealer

# one argument                                                                      
./dealer 8088

# three arguments
./dealer 8088 10 100

##### invalid arguments ######
# non-integer port number
./dealer WRONG 10

# non-integer numberofrounds
./dealer 8088 WRONG