#ifndef LAB3_PARENTUTILS_H
#define LAB3_PARENTUTILS_H
#include <dirent.h>
#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define MAX_COMMAND_LENGTH 10
#define CHILD_NAME 6
#define MAX_COUNT_OF_CHILD_PROCESSES 15
#define NO_ARGUMENT -1
enum Command {
  CMD_CREATE_PROCESS = '+',
  CMD_DELETE_LAST_PROCESS = '-',
  CMD_LIST_OF_PROCESSES = 'l',
  CMD_DELETE_ALL_PROCESSES = 'k',
  CMD_ENABLE_SHOW_STATS = 'g',
  CMD_DISABLE_SHOW_STATS = 's',
  CMD_DISABLE_SHOW_STATS_AND_ALLOW_FOR_ONE = 'p',
  CMD_EXIT = 'q'
};

typedef struct {
  enum Command command;
  long argument;
} UserInput;

typedef struct {
  pid_t pid;
  char childName[CHILD_NAME];
} ChildProcess;

void printMenu(void);
void processInput(void);
void createChildProcess(void);
void deleteLastChildProcess(void);
void listOfChildProcesses(void);
void deleteAllChildProcesses(void);
void initializeAlarmHandler(void);

#endif // LAB3_PARENTUTILS_H
