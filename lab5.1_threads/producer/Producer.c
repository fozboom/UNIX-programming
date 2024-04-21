#include "Producer.h"
#include <pthread.h>

void *producerFunction(void *arg) {
  ProducerArgs *args = (ProducerArgs *)arg;
  ProducerConsumerManager *manager = args->manager;
  int index = args->index;

  while (manager->keepRunningProducer[index]) {
    sem_wait(&manager->emptySlotsSemaphore);
    pthread_mutex_lock(&manager->queue->mutex);
    if (isQueueFull(manager->queue)) {
      pthread_mutex_unlock(&manager->queue->mutex);
      // sem_post(&manager->emptySlotsSemaphore);
      continue;
    }
    printf(GREEN_COLOR);
    Message *message = createMessage();
    addMessageToQueue(manager->queue, message);
    printMessage(message);
    printf(STANDART_COLOR);
    sem_post(&manager->usedSlotsSemaphore);
    pthread_mutex_unlock(&manager->queue->mutex);
    printf("Count added messages: %d\n", manager->queue->countAddedMessages);
    sleep(3);
  }

  free(args);
  return NULL;
}

void createProducer(ProducerConsumerManager *manager) {
  if (manager->countProducers == MAX_COUNT_OF_PRODUCERS) {
    printf("Max count of producers is reached\n");
    return;
  }
  pthread_t producerThread;
  manager->keepRunningProducer[manager->countProducers] = 1;

  ProducerArgs *args = malloc(sizeof(ProducerArgs));
  args->manager = manager;
  args->index = manager->countProducers;

  pthread_create(&producerThread, NULL, producerFunction, args);
  manager->producers[manager->countProducers++] = producerThread;
}

void deleteProducer(ProducerConsumerManager *manager, int index) {
  pthread_mutex_lock(&manager->queue->mutex);
  if (manager->countProducers == 0) {
    printf("No producers to delete\n");
    pthread_mutex_unlock(&manager->queue->mutex);
    return;
  }
  manager->keepRunningProducer[index] = 0;
  pthread_cancel(manager->producers[index]);
  pthread_detach(manager->producers[index]);
  manager->countProducers--;
  printf("Producer deleted\n");
  pthread_mutex_unlock(&manager->queue->mutex);
}

void deleteAllProducers(ProducerConsumerManager *manager) {
  while (manager->countProducers > 0) {
    deleteProducer(manager, manager->countProducers - 1);
  }
}