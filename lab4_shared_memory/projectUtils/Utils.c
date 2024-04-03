#include "Utils.h"
#include <fcntl.h>

void printQueueStatusInfo(CircularQueue *queue) {
  printf(YELLOW_COLOR);
  printf("Queue status:\n");
  printf("Queue current size: %d\n", queue->currentSize);
  printf("Queue max capacity: %d\n", queue->maxCapacity);
  printf("Queue count removed messages: %d\n", queue->countRemovedMessages);
  printf("Queue count added messages: %d\n", queue->countAddedMessages);
  printf(STANDART_COLOR);
}

void handleInput(int sharedMemoryDescriptor, CircularQueue *queue) {
  char symbol;
  printMenu();
  while (1) {
    symbol = getchar();
    getchar();
    switch (symbol) {
    case 'P':
      createProducer(sharedMemoryDescriptor);
      break;
    case 'p':
      deleteProducer();
      break;
    case 'C':
      createConsumer(sharedMemoryDescriptor);
      break;
    case 'c':
      deleteConsumer();
    case 'i':
      printQueueStatusInfo(queue);
      break;
    case 'q':
      deleteAllProducers();
      deleteAllConsumers();
      munmap(queue, SHM_SIZE);
      close(sharedMemoryDescriptor);
      shm_unlink(SHARED_MEMORY_NAME);
      return;
    }
  }
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
         "[i] - queue info\n"
         "[q] - quit\n"
         "-->   ");
}

void initializeSharedMemory(int *sharedMemoryId, CircularQueue **queue) {
  *sharedMemoryId = shm_open(SHARED_MEMORY_NAME, O_CREAT | O_RDWR, 0666);
  if (*sharedMemoryId == -1) {
    perror("shm_open");
    exit(EXIT_FAILURE);
  }

  ftruncate(*sharedMemoryId, SHM_SIZE);

  *queue = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED,
                *sharedMemoryId, 0);
  if (*queue == MAP_FAILED) {
    perror("mmap");
    exit(EXIT_FAILURE);
  }
}

void initializeSemaphores(sem_t **emptySlotsSemaphore,
                          sem_t **usedSlotsSemaphore, sem_t **queueMutex) {
  *emptySlotsSemaphore =
      sem_open(SEM_EMPTY_SLOTS, O_CREAT | O_EXCL, 0666, QUEUE_SIZE);
  if (*emptySlotsSemaphore == SEM_FAILED) {
    perror("sem_open");
    exit(EXIT_FAILURE);
  }
  *usedSlotsSemaphore = sem_open(SEM_USED_SLOTS, O_CREAT | O_EXCL, 0666, 0);
  if (*usedSlotsSemaphore == SEM_FAILED) {
    perror("sem_open");
    exit(EXIT_FAILURE);
  }
  *queueMutex = sem_open(MUTEX, O_CREAT | O_EXCL, 0666, 1);
  if (*queueMutex == SEM_FAILED) {
    perror("sem_open");
    exit(EXIT_FAILURE);
  }
}

void cleanResources() {
  shm_unlink(SHARED_MEMORY_NAME);
  sem_unlink(SEM_EMPTY_SLOTS);
  sem_unlink(SEM_USED_SLOTS);
  sem_unlink(MUTEX);
}

void closeSemaphores(sem_t *emptySlotsSemaphore, sem_t *usedSlotsSemaphore,
                     sem_t *queueMutex) {
  sem_close(emptySlotsSemaphore);
  sem_close(usedSlotsSemaphore);
  sem_close(queueMutex);
}