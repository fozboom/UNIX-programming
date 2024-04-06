#include "Utils.h"

void printQueueStatusInfo(CircularQueue *queue) {
  printf(YELLOW_COLOR);
  printf("Queue status:\n");
  printf("Queue current size: %d\n", queue->currentSize);
  printf("Queue max capacity: %d\n", queue->maxCapacity);
  printf("Queue count removed messages: %d\n", queue->countRemovedMessages);
  printf("Queue count added messages: %d\n", queue->countAddedMessages);
  printf(STANDART_COLOR);
}

void handleInput(ProducerConsumerManager *manager) {
  char symbol;
  printMenu();
  while (1) {
    symbol = getchar();
    getchar();
    switch (symbol) {
    case 'P':
      createProducer(manager);
      break;
    case 'p':
      deleteProducer(manager, manager->countProducers - 1);
      break;
    case 'C':
      createConsumer(manager);
      break;
    case 'c':
      deleteConsumer(manager, manager->countConsumers - 1);
      break;
    case 'i':
      printQueueStatusInfo(manager->queue);
      break;
    case 'q':
      printf(STANDART_COLOR);
      deleteAllProducers(manager);
      deleteAllConsumers(manager);
      destroySemaphoresAndMutex(manager);
      freeProducerConsumerManager(manager);
      return;
    }
  }
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

void initializeSemaphoresAndMutex(ProducerConsumerManager *manager) {
  sem_init(&manager->emptySlotsSemaphore, 0, QUEUE_SIZE);
  sem_init(&manager->usedSlotsSemaphore, 0, 0);
  pthread_mutex_init(&manager->queueMutex, NULL);
}

void destroySemaphoresAndMutex(ProducerConsumerManager *manager) {
  sem_destroy(&manager->emptySlotsSemaphore);
  sem_destroy(&manager->usedSlotsSemaphore);
  pthread_mutex_destroy(&manager->queueMutex);
  printf("Semaphores and mutex are destroyed\n");
}

void initializeProducerConsumerManager(ProducerConsumerManager *manager,
                                       int maxProducers, int maxConsumers) {
  manager->queue = malloc(sizeof(CircularQueue));
  initializeQueue(manager->queue);

  sem_init(&manager->emptySlotsSemaphore, 0, QUEUE_SIZE);
  sem_init(&manager->usedSlotsSemaphore, 0, 0);
  pthread_mutex_init(&manager->queueMutex, NULL);

  manager->producers = malloc(maxProducers * sizeof(pthread_t));
  manager->consumers = malloc(maxConsumers * sizeof(pthread_t));

  manager->keepRunningProducer = malloc(maxProducers * sizeof(sig_atomic_t));
  manager->keepRunningConsumer = malloc(maxConsumers * sizeof(sig_atomic_t));

  manager->countProducers = 0;
  manager->countConsumers = 0;

  for (int i = 0; i < maxProducers; i++) {
    manager->keepRunningProducer[i] = 1;
  }
  for (int i = 0; i < maxConsumers; i++) {
    manager->keepRunningConsumer[i] = 1;
  }
}

void freeProducerConsumerManager(ProducerConsumerManager *manager) {
  free(manager->producers);
  free(manager->consumers);
  free(manager->keepRunningProducer);
  free(manager->keepRunningConsumer);
  free(manager->queue);
  free(manager);
  printf("ProducerConsumerManager is freed\n");
}