all: scan

scan: scanner.c
	gcc scanner.c -o scan

scanner.c: scanner.l
	flex -o scanner.c scanner.l

clean:
	rm -f scan scanner.c
