#------------------------------------------------------------------------------
# Makefile for CMPS 101-01 Programming Assignment 3
#
#       make                   makes DigraphProperties
#       
#------------------------------------------------------------------------------

DigraphProperties : DigraphProperties.o Digraph.o List.o
	gcc -o DigraphProperties DigraphProperties.o Digraph.o List.o

DigraphProperties.o : DigraphProperties.c Digraph.h
	gcc -c -std=c99 -Wall DigraphProperties.c 


Digraph.o : Digraph.c Digraph.h
	gcc -c -std=c99 -Wall Digraph.c

clean :
	rm -f DigraphProperties DigraphProperties.o List.o
