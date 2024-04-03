#include "projectUtils/Utils.h"

int main() {
  int shmid = shm_open(SHARED_MEMORY_NAME, O_CREAT | O_RDWR, 0666);
  if (shmid == -1) {
    perror("shm_open");
    exit(EXIT_FAILURE);
  }

  ftruncate(shmid, SHM_SIZE);

  CircularQueue *queue =
      mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shmid, 0);
  if (queue == MAP_FAILED) {
    perror("mmap");
    exit(EXIT_FAILURE);
  }
  initializeQueue(queue);
  initializeSemaphorsAndMutexes();
  initializeHandler();
  char symbol;
  printMenu();
  while (1) {
    symbol = getchar();
    getchar();
    switch (symbol) {
    case 'P':
      createProducer(shmid);
      break;
    case 'p':
      deleteProducer();
      break;
    case 'C':
      createConsumer(shmid);
      break;
    case 'c':
      deleteConsumer();
      break;
    case 'q':
      deleteAllProducers();
      deleteAllConsumers();
      munmap(queue, SHM_SIZE);
      close(shmid);
      shm_unlink(SHARED_MEMORY_NAME);
      exit(EXIT_SUCCESS);
    }
  }
}