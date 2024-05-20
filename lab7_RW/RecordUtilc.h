#include "Record.h"
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#define BLUE_COLOR "\x1b[34m"
#define RESET_COLOR "\x1b[0m"
#define RED_COLOR "\x1b[31m"
#define GREEN_COLOR "\x1b[32m"

extern int file_descriptor;
extern bool record_modified;
extern Record record;
extern Record record_copy;
extern Record record_new;

typedef struct UserInput
{
    char command[10];
    int argument;
} UserInput;

int open_file(const char *filename);
void print_menu();
void print_file_content();
void processUserInput();
void exit_function();