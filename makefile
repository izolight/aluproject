# makefile for alu
CC        := gcc

vpath alu.c register.c alu-main.c

all: alu

register.o: register.c register.h
	gcc -I. -g -Wall -c register.c

alu.o: alu.c alu.h
	gcc -I. -g -Wall -c alu.c

alu-main.o: alu-main.c
	gcc -I. -g -Wall -c alu-main.c

test.o: test.c
	gcc -I. -g -Wall -c test.c

alu:	alu.o alu-main.o register.o
	gcc -g -Wall -o alu register.o alu.o alu-main.o 

createtest: alu alu.test.txt
	cat alu.test.txt | ./alu >alu.test.result

test: alu alu.test.txt
	cat alu.test.txt | ./alu | diff - alu.test.result

mytest: register.o alu.o test.o
	gcc -g -Wall -o mytest register.o alu.o test.o -lm -lm

prettypdf: alu.c
	enscript -2rG -Ec -MA4 -o alu.c.pdf alu.c

prettyprint: alu.c
	enscript -2rG -Ec -MA4 alu.c

clean:
	rm *.o alu mytest
