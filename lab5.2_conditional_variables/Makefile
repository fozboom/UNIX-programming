CC = gcc
CFLAGS = -W -Wall -Wno-unused-parameter -Wno-unused-variable -std=c11 -pedantic -D_XOPEN_SOURCE=700 -pthread
LDFLAGS = -lpthread

SRC = main.c \
	circularQueue/CircularQueue.c \
	consumer/Consumer.c \
	message/Message.c \
	producer/Producer.c \
	projectUtils/Utils.c 

OBJ = $(SRC:.c=.o)

all: program

program: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c defines.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) program

run:
	$(MAKE) all
	./program
	$(MAKE) clean

.PHONY: all clean run