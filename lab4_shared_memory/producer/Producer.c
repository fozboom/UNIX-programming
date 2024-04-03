#include "Producer.h"
#include "../projectUtils/Utils.h"
#include <sys/mman.h>

volatile sig_atomic_t keepRunningProducer = 1;

pid_t producersPid[MAX_COUNT_OF_PRODUCERS];
size_t producersCount = 0;

void createProducer() {
  if (producersCount == MAX_COUNT_OF_PRODUCERS) {
    printf("Max count of producers is reached\n");
    return;
  }
  pid_t pid = fork();

  if (pid == -1) {
    perror("producer fork");
    exit(EXIT_FAILURE);
  } else if (pid == 0) {
    srand(getpid());
  } else {
    producersPid[producersCount++] = pid;
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

  while (keepRunningProducer) {
    printf(RED_COLOR);
    sem_wait(emptySlotsSemaphore);
    sem_wait(queueMutex);

    Message *message;
    message = createMessage();

    addMessageToQueue(queue, message);
    printMessage(message);

    sem_post(queueMutex);
    sem_post(usedSlotsSemaphore);
    printf(STANDART_COLOR);
    printf("Count added messages: %u\n", queue->countAddedMessages);

    sleep(4);
  }
  printf(STANDART_COLOR);

  munmap(queue, SHM_SIZE);
  close(shmid);
  exit(EXIT_SUCCESS);
}
void deleteProducer() {
  if (producersCount == 0) {
    return;
  }
  printf("Producer with pid %d was deleted\n",
         producersPid[producersCount - 1]);
  kill(producersPid[--producersCount], SIGUSR2);
}

void deleteAllProducers() {
  while (producersCount > 0) {
    deleteProducer();
  }
}