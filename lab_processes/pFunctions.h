#include <locale.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int comparator(const void *str1, const void *str2);
void printSortingEnviron (char* environ[]);
char* parsingEnviron (char * envp[], const char* parametrName);