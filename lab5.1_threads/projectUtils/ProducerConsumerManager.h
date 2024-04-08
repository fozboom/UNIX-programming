#ifndef PRODUCER_CONSUMER_MANAGER_H
#define PRODUCER_CONSUMER_MANAGER_H

#include "../circularQueue/CircularQueue.h"
#include "../defines.h"
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>

typedef struct {
  sem_t emptySlotsSemaphore;
  sem_t usedSlotsSemaphore;
  pthread_t *producers;
  pthread_t *consumers;
  CircularQueue *queue;
  int countProducers;
  int countConsumers;
  sig_atomic_t *keepRunningProducer;
  sig_atomic_t *keepRunningConsumer;
} ProducerConsumerManager;

#endif