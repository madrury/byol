all: mlisp 

mlisp:
	gcc -std=c99 -o mlisp -Wall -ledit main.c

