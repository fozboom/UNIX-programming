CC=gcc
CFLAGS=-W -Wall -Wno-unused-parameter -Wno-unused-variable -std=c11 -pedantic

.PHONY: all clean run

all: parent child

parent: parent.o pFunctions.o
	$(CC) $(CFLAGS) -o parent parent.o pFunctions.o

child: child.o
	$(CC) $(CFLAGS) -o child child.o

parent.o: parent.c pFunctions.h
	$(CC) $(CFLAGS) -c -o parent.o parent.c

child.o: child.c
	$(CC) $(CFLAGS) -c -o child.o child.c

pFunctions.o: pFunctions.c pFunctions.h
	$(CC) $(CFLAGS) -c -o pFunctions.o pFunctions.c

run:
	make child
	make parent
	CHILD_PATH=./child LC_COLLATE=C ./parent environ.txt
	make clean

clean:
	rm -f parent child *.o
