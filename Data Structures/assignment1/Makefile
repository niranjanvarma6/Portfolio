CC = gcc
CFLAGS = -std=c99

default: test

test: man.c dynarray.o list.o
	$(CC) $(CFLAGS) list.c dynarray.c man.c -o test
dynarray.o: dynarray.h dynarray.h
	$(CC) $(CFLAGS) -c dynarray.c
list.o: list.c list.h
	$(CC) $(CFLAGS) -c list.c 

clean:
	$(RM) test *.o *~