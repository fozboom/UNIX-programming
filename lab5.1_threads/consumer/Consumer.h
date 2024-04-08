#ifndef CONSUMER_H
#define CONSUMER_H
#include "../circularQueue/CircularQueue.h"
#include "../defines.h"
#include "../projectUtils/ProducerConsumerManager.h"
#include <unistd.h>

typedef struct {
  ProducerConsumerManager *manager;
  int index;
} ConsumerArgs;

void *consumerFunction(void *arg);
void createConsumer(ProducerConsumerManager *manager);
void deleteConsumer(ProducerConsumerManager *manager, int index);
void deleteAllConsumers(ProducerConsumerManager *manager);

#endif
