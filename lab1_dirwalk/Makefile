CC = gcc
CFLAGS = -Wall -Wextra
TARGET = dirwalk
OBJS = main.o functions.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c functions.h
	$(CC) $(CFLAGS) -c main.c

functions.o: functions.c functions.h
	$(CC) $(CFLAGS) -c functions.c

clean:
	rm -f $(TARGET) $(OBJS)

run: $(TARGET)
	./$(TARGET) /home/daniel/ -lds