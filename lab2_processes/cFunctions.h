#ifndef CFUNCTIONS_H
#define CFUNCTIONS_H
#include "pFunctions.h"

extern char **environ;

#define OPTION_GETENV '+'
#define OPTION_ENVP '*'
#define OPTION_ENVIRON '&'

void printProcessInfo(char* processName);
void handleFile(char* fileName, char* envp[], char option);

#endif