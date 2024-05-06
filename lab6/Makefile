CC=gcc
CFLAGS=-W -Wall -Wno-unused-parameter -Wno-unused-variable -std=c11 -pedantic -pthread -D_XOPEN_SOURCE=600
DEPS = index_data.h sort_index_utils.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

index_generator: index_generator.o
	$(CC) -o $@ $^ $(CFLAGS)


sort_index: sort_index.o sort_index_utils.o
	$(CC) -o $@ $^ $(CFLAGS)


all: index_generator sort_index


.PHONY: clean all

clean:
	rm -f *.o index_generator read_index sort_index