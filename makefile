CC = gcc
CFLAGS = -Wall -std=c99
LDFLAGS = -lm -pthread

prime: prime.c
	${CC} ${CFLAGS} prime.c ${LDFLAGS} -o prime

clean: 
	rm -f *.o
