#include "parentUtils.h"

size_t countCreatedChildProcesses = 0;
ChildProcess createdChildProcessesPids[MAX_COUNT_OF_CHILD_PROCESSES];

int main(int argc, char *argv[]) {
  initializeAlarmHandler();
  printMenu();
  processInput();
  return 0;
}
