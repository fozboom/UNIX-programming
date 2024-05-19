#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct Record {
    char name[80];
    char address[80];
    uint8_t semester;
} Record;