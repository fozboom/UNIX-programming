#include "Utils.h"

sem_t emptySlotsSemaphore;
sem_t usedSlotsSemaphore;
sem_t queueMutex;

void initializeSemaphorsAndMutexes() {
  sem_init(&emptySlotsSemaphore, 0, QUEUE_SIZE);
  sem_init(&usedSlotsSemaphore, 0, 0);
  sem_init(&queueMutex, 0, 1);
}

void handleSIGUSR2(int signal, siginfo_t *info, void *ptr) {
  keepRunningProducer = 0;
}

void handleSIGUSR1(int signal, siginfo_t *info, void *ptr) {
  keepRunningConsumer = 0;
}

void initializeHandler() {
  struct sigaction act;
  act.sa_sigaction = &handleSIGUSR1;
  act.sa_flags = SA_SIGINFO | SA_RESTART | SA_NOCLDWAIT;
  sigaction(SIGINT, &act, NULL);

  act.sa_sigaction = &handleSIGUSR2;
  sigaction(SIGUSR2, &act, NULL);
}

void printMenu() {
  printf("Choice option:\n"
         "[P] - create producer\n"
         "[p] - delete producer\n"
         "[C] - create consumer\n"
         "[c] - delete consumer\n"
         "[q] - quit\n"
         "-->   ");
}