# makefile for alu
CC        := clang

vpath alu.c register.c alu-main.c

all: alu

register.o: register.c register.h
	clang -I. -g -Wall -c register.c

alu.o: alu.c alu.h
	clang -I. -g -Wall -c alu.c

alu-main.o: alu-main.c
	clang -I. -g -Wall -c alu-main.c

alu:	alu.o alu-main.o register.o
	clang -g -Wall -o alu register.o alu.o alu-main.o 

createtest: alu alu.test.txt
	cat alu.test.txt | ./alu >alu.test.result

test: alu alu.test.txt
	cat alu.test.txt | ./alu | diff - alu.test.result

prettypdf: alu.c
	enscript -2rG -Ec -MA4 -o alu.c.pdf alu.c

prettyprint: alu.c
	enscript -2rG -Ec -MA4 alu.c

clean:
	rm *.o alu
