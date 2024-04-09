#include "Consumer.h"
#include <pthread.h>

void *consumerFunction(void *arg) {
  ConsumerArgs *args = (ConsumerArgs *)arg;
  ProducerConsumerManager *manager = args->manager;
  int index = args->index;

  while (manager->keepRunningConsumer[index]) {
    pthread_mutex_lock(&manager->queue->mutex);
    while (manager->queue->countAddedMessages -
               manager->queue->countRemovedMessages <=
           0) {
      if (!manager->keepRunningConsumer[index]) {
        pthread_cond_signal(&manager->queue->notFull);
        pthread_mutex_unlock(&manager->queue->mutex);
        break;
      }
      pthread_cond_wait(&manager->queue->notEmpty, &manager->queue->mutex);
    }
    if (!manager->keepRunningConsumer[index]) {
      break;
    }

    printf(RED_COLOR);

    removeMessageFromQueue(manager->queue);

    printf("Count removed messages: %d\n",
           manager->queue->countRemovedMessages);

    pthread_cond_signal(&manager->queue->notFull);
    pthread_mutex_unlock(&manager->queue->mutex);
    sleep(3);
  }
  printf(STANDART_COLOR);
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
  if (manager->countConsumers == 0) {
    printf("No consumers to delete\n");
    return;
  }
  manager->keepRunningConsumer[index] = 0;
  pthread_cond_broadcast(&manager->queue->notEmpty);
  pthread_cancel(manager->consumers[index]);
  manager->countConsumers--;
  printf("Consumer deleted\n");
}

void deleteAllConsumers(ProducerConsumerManager *manager) {
  while (manager->countConsumers > 0) {
    deleteConsumer(manager, manager->countConsumers - 1);
  }
}