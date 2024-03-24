#include "parentUtils.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

extern ChildProcess createdChildProcessesPids[MAX_COUNT_OF_CHILD_PROCESSES];
extern size_t countCreatedChildProcesses;

void createChildProcess(void) {
  if (countCreatedChildProcesses == MAX_COUNT_OF_CHILD_PROCESSES) {
    printf("Max count of child processes reached\n");
  }

  pid_t pid = fork();
  if (pid == -1) {
    perror("fork failed\n");
    deleteAllChildProcesses();
    exit(EXIT_FAILURE);
  } else if (pid == 0) {
    char childName[8];
    sprintf(childName, "C_%02zu", countCreatedChildProcesses);
    if (execl("./child_exec", childName, NULL) == -1) {
      deleteAllChildProcesses();
      perror("execl failed\n");
      exit(EXIT_FAILURE);
    }
  } else {
    createdChildProcessesPids[countCreatedChildProcesses].pid = pid;
    sprintf(createdChildProcessesPids[countCreatedChildProcesses].childName,
            "C_%02zu", countCreatedChildProcesses);
    countCreatedChildProcesses++;
  }
}

void deleteLastChildProcess() {
  if (countCreatedChildProcesses == 0) {
    printf("No child processes to delete\n");
    return;
  }
  ChildProcess child = createdChildProcessesPids[--countCreatedChildProcesses];
  kill(child.pid, SIGTERM);
  int status;
  waitpid(child.pid, &status, 0);
  printf("Deleted child process with PID: %d. Remaining children: %zu\n",
         child.pid, countCreatedChildProcesses);
}

void listOfChildProcesses(void) {
  printf("Parent process PID: %d\n", getpid());
  printf("Child processes PIDs: ");
  for (size_t i = 0; i < countCreatedChildProcesses; i++) {
    printf("%d ", createdChildProcessesPids[i].pid);
  }
  printf("\n");
}

void deleteAllChildProcesses() {
  int status;
  for (size_t i = 0; i < countCreatedChildProcesses; i++) {
    ChildProcess child = createdChildProcessesPids[i];
    kill(child.pid, SIGTERM);
    waitpid(child.pid, &status, 0);
    printf("Deleted child process with PID: %d\n", child.pid);
  }
  countCreatedChildProcesses = 0;
}

void sendSignalToBlockStatisticsOutput(ChildProcess child) {
  kill(child.pid, SIGUSR1);
}

void sendSignalToAllowStatisticsOutput(ChildProcess child) {
  kill(child.pid, SIGUSR2);
}

void blockStatisticsOutputForAllProcess() {
  for (size_t i = 0; i < countCreatedChildProcesses; i++) {
    sendSignalToBlockStatisticsOutput(createdChildProcessesPids[i]);
  }
}

void allowStatisticsOutputForAllProcess() {
  for (size_t i = 0; i < countCreatedChildProcesses; i++) {
    sendSignalToAllowStatisticsOutput(createdChildProcessesPids[i]);
  }
}

void printMenu() {
  printf("[+] - создать дочерний процесс\n"
         "[-] - удалить последний дочерний процесс\n"
         "[l] - выводится перечень родительских и дочерних процессов.\n"
         "[k] - удалить все дочерние\n"
         "[s] - запретить всем дочерним процессам выводить статистику\n"
         "[g] - разрешить всем дочерним процессам выводить статистику\n"
         "[s] <num> - запретить C_<num> выводить статистику\n"
         "[g] <num> - разрешить C_<num> выводить статистику\n"
         "[p] <num> - показывать статистику C_{num} и запретить другим "
         "процессам показывать статистику\n"
         "[q] - выход\n"
         "---> ");
}

char *readCommand() {
  char *commandName = malloc(MAX_COMMAND_LENGTH * sizeof(char));
  fgets(commandName, MAX_COMMAND_LENGTH, stdin);
  rewind(stdin);
  return commandName;
}

UserInput parseUserInput(char *commandName) {
  UserInput userInput;
  char *token = strtok(commandName, "_");
  userInput.command = (enum Command)token[0];
  token = strtok(NULL, "_");
  userInput.argument = token ? strtol(token, NULL, 10) : -1;
  return userInput;
}

void processListOfProcessesCommand() {
  printf("\nCommand 'l' was entered\n");
  listOfChildProcesses();
  char command[20];
  sprintf(command, "pstree -p %d", getpid());
  system(command);
}

void processDisableShowStatsCommand(UserInput userInput) {
  if (userInput.argument != -1) {
    printf("\nCommand 's_k' was entered\n");
    sendSignalToBlockStatisticsOutput(
        createdChildProcessesPids[userInput.argument]);
  } else {
    printf("\nCommand 's' was entered\n");
    blockStatisticsOutputForAllProcess();
  }
}

void processEnableShowStatsCommand(UserInput userInput) {
  if (userInput.argument != -1) {
    printf("\nCommand 'g_k' was entered\n");
    sendSignalToAllowStatisticsOutput(
        createdChildProcessesPids[userInput.argument]);
  } else {
    printf("\nCommand 'g' was entered\n");
    allowStatisticsOutputForAllProcess();
  }
}

void processDisableShowStatsAndAllowForOneCommand(UserInput userInput) {
  if (userInput.argument != -1) {
    printf("\nCommand 'p_k' was entered\n");
    blockStatisticsOutputForAllProcess();
    nanosleep(&(struct timespec){.tv_sec = 0, .tv_nsec = 1000000}, NULL);
    sendSignalToAllowStatisticsOutput(
        createdChildProcessesPids[userInput.argument]);
    alarm(5);
  }
}

void processInput(void) {
  while (1) {
    char *commandName = readCommand();
    UserInput userInput = parseUserInput(commandName);
    free(commandName);

    switch (userInput.command) {
    case CMD_CREATE_PROCESS:
      printf("\nCommand '+' was entered\n");
      createChildProcess();
      break;
    case CMD_DELETE_LAST_PROCESS:
      printf("\nCommand '-' was entered\n");
      deleteLastChildProcess();
      break;
    case CMD_LIST_OF_PROCESSES:
      processListOfProcessesCommand();
      break;
    case CMD_DELETE_ALL_PROCESSES:
      deleteAllChildProcesses();
      break;
    case CMD_DISABLE_SHOW_STATS:
      processDisableShowStatsCommand(userInput);
      break;
    case CMD_ENABLE_SHOW_STATS:
      processEnableShowStatsCommand(userInput);
      break;
    case CMD_DISABLE_SHOW_STATS_AND_ALLOW_FOR_ONE:
      processDisableShowStatsAndAllowForOneCommand(userInput);
      break;
    case CMD_EXIT:
      deleteAllChildProcesses();
      exit(EXIT_SUCCESS);
    default:
      break;
    }
  }
}

void parentHandler(int signal, siginfo_t *siginfo, void *context) {
  if (signal == SIGALRM) {
    allowStatisticsOutputForAllProcess();
  }
  if (signal == SIGINT) {
    deleteAllChildProcesses();
    exit(EXIT_SUCCESS);
  }
}

void initializeAlarmHandler(void) {
  struct sigaction sa;
  sa.sa_sigaction = parentHandler;
  sa.sa_flags = SA_SIGINFO;
  sigaction(SIGALRM, &sa, NULL);
  sigaction(SIGINT, &sa, NULL);
}
