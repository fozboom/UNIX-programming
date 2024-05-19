#include <stdio.h>
#include <stdlib.h>
#define COUNT_RECORDS 10

typedef struct Record
{
    char name[80];
    char address[80];
    int semester;
} Record;