#include "Producer.h"
#include "../projectUtils/Utils.h"
#include <sys/mman.h>

volatile sig_atomic_t keepRunningProducer = 1;

pid_t producersPid[MAX_COUNT_OF_PRODUCERS];
size_t producersCount = 0;

void createProducer() {
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

  while (keepRunningProducer) {
    printf("shmid - %d\n", shmid);
    Message *message = mmap(NULL, sizeof(Message), PROT_READ | PROT_WRITE,
                            MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    message = createMessage();
    sem_wait(&emptySlotsSemaphore);
    sem_wait(&queueMutex);

    printf("Producer\n");
    addMessageToQueue(queue, message);
    printMessage(message);
    printf("%d\n", queue->currentSize);

    sem_post(&queueMutex);
    sem_post(&usedSlotsSemaphore);

    sleep(4);
    munmap(message, sizeof(Message));
  }

  munmap(queue, SHM_SIZE);

  close(shmid);

  exit(EXIT_SUCCESS);
}
void deleteProducer() {
  if (producersCount == 0) {
    return;
  }

  kill(producersPid[--producersCount], SIGUSR2);
}

void deleteAllProducers() {
  while (producersCount > 0) {
    deleteProducer();
  }
}
