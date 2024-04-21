#include "Producer.h"
#include <pthread.h>

void *producerFunction(void *arg) {

  ProducerArgs *args = (ProducerArgs *)arg;
  ProducerConsumerManager *manager = args->manager;
  int index = args->index;

  while (manager->keepRunningProducer[index]) {
    pthread_mutex_lock(&manager->queue->mutex);
    while (manager->keepRunningProducer[index] && isQueueFull(manager->queue)) {
      pthread_cond_wait(&manager->queue->canWrite, &manager->queue->mutex);
    }
    if (!manager->keepRunningProducer[index]) {
      pthread_mutex_unlock(&manager->queue->mutex);
      break;
    }

    printf(GREEN_COLOR);
    Message *message = createMessage();
    addMessageToQueue(manager->queue, message);
    printMessage(message);
    printf(STANDART_COLOR);
    pthread_cond_signal(&manager->queue->canRead);
    pthread_mutex_unlock(&manager->queue->mutex);
    printf("Count added messages: %d\n", manager->queue->countAddedMessages);
    sleep(3);
  }

  free(args);
  pthread_exit(NULL);
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
    pthread_mutex_unlock(&manager->queue->mutex);
    return;
  }
  manager->keepRunningProducer[index] = 0;
  pthread_cond_broadcast(&manager->queue->canWrite);
  pthread_join(manager->producers[index], NULL);
  manager->countProducers--;
  printf("Producer deleted\n");
}

void deleteAllProducers(ProducerConsumerManager *manager) {
  while (manager->countProducers > 0) {
    deleteProducer(manager, manager->countProducers - 1);
  }
}