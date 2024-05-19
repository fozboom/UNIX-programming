CC=gcc
CFLAGS= -W -Wall -Wno-unused-parameter -Wno-unused-variable -std=c11 
DEPS = Record.h RecordUtilc.h
OBJ1 = RecordsGenerator.o
OBJ2 = main.o RecordUtilc.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

RecordsGenerator: $(OBJ1)
	$(CC) -o $@ $^ $(CFLAGS)

main: $(OBJ2)
	$(CC) -o $@ $^ $(CFLAGS)

all: RecordsGenerator main

.PHONY: clean all

clean:
	rm -f *.o main RecordGenerator