#ifndef MESSAGE_H
#define MESSAGE_H
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define DATA_SIZE 256
typedef struct {
  uint8_t type;
  uint16_t hash;
  uint8_t size;
  char data[DATA_SIZE];
} Message;

Message *createMessage();
void printMessage(Message *message);

#endif
