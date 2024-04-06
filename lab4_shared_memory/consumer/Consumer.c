#include "Consumer.h"
#include "../projectUtils/Utils.h"
#include <stdio.h>
#include <sys/mman.h>

pid_t consumersPid[MAX_COUNT_OF_CONSUMERS];
size_t consumersCount = 0;

volatile sig_atomic_t keepRunningConsumer = 1;

void createConsumer() {
  if (consumersCount == MAX_COUNT_OF_CONSUMERS) {
    printf("Max count of consumers is reached\n");
    return;
  }
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
  int res;
  while (keepRunningConsumer) {
    printf(GREEN_COLOR);
    sem_wait(usedSlotsSemaphore);
    sem_wait(queueMutex);

    Message *message = removeMessageFromQueue(queue);
    printMessage(message);

    sem_post(queueMutex);
    sem_post(emptySlotsSemaphore);
    printf(STANDART_COLOR);
    printf("Count removed messages: %d\n", queue->countRemovedMessages);
    fflush(stdout);
    free(message->data);
    free(message);
    sleep(2);
  }
  printf(STANDART_COLOR);
  munmap(queue, SHM_SIZE);
  close(shmid);
  exit(EXIT_SUCCESS);
}

void deleteConsumer() {
  if (consumersCount == 0) {
    return;
  }
  printf("Consumer with pid %d was deleted\n",
         consumersPid[consumersCount - 1]);
  kill(consumersPid[--consumersCount], SIGUSR1);
}

void deleteAllConsumers() {
  while (consumersCount > 0) {
    deleteConsumer();
  }
}