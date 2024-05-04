CC=gcc
CFLAGS=-W -Wall -Wno-unused-parameter -Wno-unused-variable -std=c11 -pedantic
DEPS = index_data.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

index_generator: index_generator.o
	$(CC) -o $@ $^ $(CFLAGS)

read_index: read_index.o
	$(CC) -o $@ $^ $(CFLAGS)

all: index_generator read_index


.PHONY: clean all

clean:
	rm -f *.o index_generator read_index