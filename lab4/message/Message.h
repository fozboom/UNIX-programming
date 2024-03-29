#include <stdint.h>
#include <stdio.h>

#define DATA_SIZE (((256 + 3) / 4) * 4)
typedef struct {
  uint8_t type;
  uint16_t hash;
  uint8_t size;
  char data[DATA_SIZE];
} Message;