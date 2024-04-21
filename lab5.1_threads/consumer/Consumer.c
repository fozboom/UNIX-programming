#include "Consumer.h"
#include <pthread.h>

void *consumerFunction(void *arg) {
  ConsumerArgs *args = (ConsumerArgs *)arg;
  ProducerConsumerManager *manager = args->manager;
  int index = args->index;

  while (manager->keepRunningConsumer[index]) {
    sem_wait(&manager->usedSlotsSemaphore);
    pthread_mutex_lock(&manager->queue->mutex);
    if (isQueueEmpty(manager->queue)) {
      pthread_mutex_unlock(&manager->queue->mutex);
      // sem_post(&manager->usedSlotsSemaphore);
      continue;
    }
    printf(RED_COLOR);
    removeMessageFromQueue(manager->queue);
    printf("Count removed messages: %d\n",
           manager->queue->countRemovedMessages);
    printf(STANDART_COLOR);

    sem_post(&manager->emptySlotsSemaphore);
    pthread_mutex_unlock(&manager->queue->mutex);

    sleep(3);
  }

  free(args);
  return NULL;
}

void createConsumer(ProducerConsumerManager *manager) {
  if (manager->countConsumers == MAX_COUNT_OF_CONSUMERS) {
    printf("Max count of consumers is reached\n");
    return;
  }
  pthread_t consumerThread;
  manager->keepRunningConsumer[manager->countConsumers] = 1;

  ConsumerArgs *args = malloc(sizeof(ConsumerArgs));
  args->manager = manager;
  args->index = manager->countConsumers;

  pthread_create(&consumerThread, NULL, consumerFunction, args);
  manager->consumers[manager->countConsumers++] = consumerThread;
}

void deleteConsumer(ProducerConsumerManager *manager, int index) {
  pthread_mutex_lock(&manager->queue->mutex);
  if (manager->countConsumers == 0) {
    printf("No consumers to delete\n");
    pthread_mutex_unlock(&manager->queue->mutex);
    return;
  }
  manager->keepRunningConsumer[index] = 0;

  pthread_cancel(manager->consumers[index]);
  pthread_detach(manager->consumers[index]);
  manager->countConsumers--;
  printf("Consumer deleted\n");
  pthread_mutex_unlock(&manager->queue->mutex);
}

void deleteAllConsumers(ProducerConsumerManager *manager) {
  while (manager->countConsumers > 0) {
    deleteConsumer(manager, manager->countConsumers - 1);
  }
}