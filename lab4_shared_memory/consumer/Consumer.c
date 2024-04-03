#include "Consumer.h"
#include "../projectUtils/Utils.h"
#include <sys/mman.h>

pid_t consumersPid[MAX_COUNT_OF_CONSUMERS];
size_t consumersCount = 0;

volatile sig_atomic_t keepRunningConsumer = 1;

void createConsumer() {
  pid_t pid = fork();

  if (pid == -1) {
    perror("consumer fork");
    exit(EXIT_FAILURE);
  } else if (pid == 0) {
    srand(getpid());
  } else {
    consumersPid[consumersCount++] = pid;
    return;
  }
  int shmid = shm_open(SHARED_MEMORY_NAME, O_CREAT | O_RDWR, 0666);
  if (shmid == -1) {
    perror("shm_open");
    exit(EXIT_FAILURE);
  }
  CircularQueue *queue =
      mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shmid, 0);
  if (queue == MAP_FAILED) {
    perror("mmap");
    exit(EXIT_FAILURE);
  }
  sem_t *emptySlotsSemaphore = sem_open(SEM_EMPTY_SLOTS, QUEUE_SIZE);
  if (emptySlotsSemaphore == SEM_FAILED) {
    perror("sem_open");
    exit(EXIT_FAILURE);
  }
  sem_t *usedSlotsSemaphore = sem_open(SEM_USED_SLOTS, 0);
  if (usedSlotsSemaphore == SEM_FAILED) {
    perror("sem_open");
    exit(EXIT_FAILURE);
  }
  sem_t *queueMutex = sem_open(MUTEX, 1);
  if (queueMutex == SEM_FAILED) {
    perror("sem_open");
    exit(EXIT_FAILURE);
  }

  while (keepRunningConsumer) {
    printf("shmid consumer - %d\n", shmid);
    sem_wait(usedSlotsSemaphore);
    sem_wait(queueMutex);

    Message *message = removeMessageFromQueue(queue);

    printf("Consumer currentSize - %d\n", queue->currentSize);
    printf("Consumer\n");
    printMessage(message);

    sem_post(queueMutex);
    sem_post(emptySlotsSemaphore);

    sleep(3);
  }

  munmap(queue, SHM_SIZE);
  close(shmid);
  exit(EXIT_SUCCESS);
}

void deleteConsumer() {
  if (consumersCount == 0) {
    return;
  }

  kill(consumersPid[--consumersCount], SIGUSR1);
}

void deleteAllConsumers() {
  while (consumersCount > 0) {
    deleteConsumer();
  }
}