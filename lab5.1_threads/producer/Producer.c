#include "Producer.h"

void *producerFunction(void *arg) {
  ProducerArgs *args = (ProducerArgs *)arg;
  ProducerConsumerManager *manager = args->manager;
  int index = args->index;

  while (manager->keepRunningProducer[index]) {
    printf(GREEN_COLOR);
    sem_wait(&manager->emptySlotsSemaphore);
    pthread_mutex_lock(&manager->queue->mutex);

    Message *message = createMessage();
    addMessageToQueue(manager->queue, message);
    printMessage(message);

    pthread_mutex_unlock(&manager->queue->mutex);
    sem_post(&manager->usedSlotsSemaphore);
    printf("Count added messages: %d\n", manager->queue->countAddedMessages);
    sleep(2);
  }
  printf(STANDART_COLOR);
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
  if (manager->countProducers == 0) {
    printf("No producers to delete\n");
    return;
  }
  manager->keepRunningProducer[index] = 0;
  pthread_cancel(manager->producers[index]);
  pthread_join(manager->producers[index], NULL);
  manager->countProducers--;
  printf("Producer deleted\n");
}

void deleteAllProducers(ProducerConsumerManager *manager) {
  while (manager->countProducers > 0) {
    deleteProducer(manager, manager->countProducers - 1);
  }
}