all: mlisp 

mlisp:
	gcc -std=c99 -o mlisp -Wall -ledit -lm mpc/mpc.c parser.c main.c

