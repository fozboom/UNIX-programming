CC = gcc
CFLAGS = -W -Wall -Wno-unused-parameter -Wno-unused-variable -std=c11 -pedantic -D_XOPEN_SOURCE=700
DEPS = parent/parentUtils.h child/childProcessFunctions.h
OBJ1 = parent/parent.o parent/parentUtils.o
OBJ2 = child/child.o child/childProcessFunctions.o
EXEC1 = parent_exec
EXEC2 = child_exec

all: $(EXEC1) $(EXEC2)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXEC1): $(OBJ1)
	$(CC) -o $@ $^ $(CFLAGS)

$(EXEC2): $(OBJ2)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean run

clean:
	rm -f $(OBJ1) $(OBJ2) $(EXEC1) $(EXEC2)

run:
	make
	make child_exec
	./parent_exec
	make clean