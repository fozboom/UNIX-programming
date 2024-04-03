#ifndef MESSAGE_H
#define MESSAGE_H
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  uint8_t type;
  uint16_t hash;
  uint8_t size;
  char *data;
} Message;

Message *createMessage();
void printMessage(Message *message);

#endif
