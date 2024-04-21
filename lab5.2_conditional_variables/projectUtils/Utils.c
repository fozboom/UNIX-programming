#include "Utils.h"
#include <stdio.h>

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
    scanf("%c", &symbol);
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
      ;
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
      pthread_mutex_lock(&manager->queue->mutex);
      printQueueStatusInfo(manager->queue);
      pthread_mutex_unlock(&manager->queue->mutex);
      break;
    case '+':
      pthread_mutex_lock(&manager->queue->mutex);
      if (increaseQueueSize(manager->queue)) {
        pthread_cond_signal(&manager->queue->canWrite);
      }
      pthread_mutex_unlock(&manager->queue->mutex);
      break;
    case '-':
      pthread_mutex_lock(&manager->queue->mutex);
      if (decreaseQueueSize(manager->queue)) {
        pthread_cond_signal(&manager->queue->canRead);
      }
      pthread_mutex_unlock(&manager->queue->mutex);
      break;
    case 'q':
      printf(STANDART_COLOR);
      deleteAllProducers(manager);
      deleteAllConsumers(manager);
      freeProducerConsumerManager(manager);
      return;
    default:
      printf("Invalid option\n");
      break;
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

void initializeProducerConsumerManager(ProducerConsumerManager *manager,
                                       int maxProducers, int maxConsumers) {
  manager->queue = malloc(sizeof(CircularQueue));
  initializeQueue(manager->queue);

  pthread_mutex_init(&manager->queue->mutex, NULL);
  pthread_cond_init(&manager->queue->canRead, NULL);
  pthread_cond_init(&manager->queue->canWrite, NULL);

  manager->producers = malloc(maxProducers * sizeof(pthread_t));
  manager->consumers = malloc(maxConsumers * sizeof(pthread_t));

  manager->keepRunningProducer = malloc(maxProducers * sizeof(sig_atomic_t));
  manager->keepRunningConsumer = malloc(maxConsumers * sizeof(sig_atomic_t));

  manager->countProducers = 0;
  manager->countConsumers = 0;

  for (int i = 0; i < maxProducers; i++) {
    manager->keepRunningProducer[i] = 0;
  }
  for (int i = 0; i < maxConsumers; i++) {
    manager->keepRunningConsumer[i] = 0;
  }
}

void destroyProducerConsumerManager(ProducerConsumerManager *manager) {
  pthread_mutex_destroy(&manager->queue->mutex);
  pthread_cond_destroy(&manager->queue->canRead);
  pthread_cond_destroy(&manager->queue->canWrite);
  printf("Mutex and condition variables are destroyed\n");
}

void freeProducerConsumerManager(ProducerConsumerManager *manager) {
  destroyProducerConsumerManager(manager);
  freeQueue(manager->queue);
  free(manager->queue);
  free(manager->producers);
  free(manager->consumers);
  free(manager->keepRunningProducer);
  free(manager->keepRunningConsumer);
  free(manager);
  printf("ProducerConsumerManager is freed\n");
}
