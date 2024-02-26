#include <locale.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define GREEN_COLOR "\033[1;32m"
#define DEFAULT_COLOR "\033[0m"
#define BUFFER_SIZE 80
#define PURPLE_COLOR "\033[0;35m"

int comparator(const void *str1, const void *str2);
void printSortingEnviron (char* environ[]);
char* parsingEnviron (char * envp[], const char* parametrName);