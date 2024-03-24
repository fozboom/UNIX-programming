#include "parentUtils.h"

pid_t createdChildProcessesPids[MAX_COUNT_OF_CHILD_PROCESSES];
size_t countCreatedChildProcesses = 0;

int main(int argc, char *argv[]) {
    initializeAlarmHandler();
    printMenu();
    processInput();
    return 0;
}
