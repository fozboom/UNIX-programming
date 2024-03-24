#ifndef PFUNCTIONS_H
#define PFUNCTIONS_H

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
#define BLUE_COLOR "\033[0;34m"

#define CHILD_PATH "CHILD_PATH"
#define CHILD_PROGRAM_NAME_FORMAT "child_%02zu"
#define OPTION_GETENV '+'
#define OPTION_ENVP '*'
#define OPTION_ENVIRON '&'
#define OPTION_EXIT 'q'

extern char **environ;

int comparator(const void *str1, const void *str2);
void printSortingEnviron (char* environ[]);
char* parsingEnviron (char * envp[], const char* parametrName);
char** createReduceEnv(char * envp[], const char* nameFileWithNamesEnvironParametrs);
void printMenu();

#endif //PFUNCTIONS_H