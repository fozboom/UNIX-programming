CC=gcc
CFLAGS=-W -Wall -Wno-unused-parameter -Wno-unused-variable -std=c11 -pedantic
DEPS = index_data.h
OBJ = index_generator.o 

%.o: %.c $(DEPS)
    $(CC) -c -o $@ $< $(CFLAGS)

index_generator: $(OBJ)
    $(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
    rm -f *.o index_generator