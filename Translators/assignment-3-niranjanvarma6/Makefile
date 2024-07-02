CC=gcc --std=c99

all: parse

parse: parser.c scanner.c hash.o
	$(CC) parser.c scanner.c hash.o -o parse

scanner.c: scanner.l
	flex -o scanner.c scanner.l

parser.c parser.h: parser.y
	bison -d -o parser.c parser.y

hash.o: hash.c hash.h
	$(CC) hash.c -c -o hash.o

clean:
	rm -f parse scanner.c parser.c parser.h *.o
