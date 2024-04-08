#ifndef PRODUCER_H
#define PRODUCER_H

#include "../projectUtils/ProducerConsumerManager.h"
#include <unistd.h>

typedef struct {
  ProducerConsumerManager *manager;
  int index;
} ProducerArgs;

void *producerFunction(void *arg);
void createProducer(ProducerConsumerManager *manager);
void deleteProducer(ProducerConsumerManager *manager, int index);
void deleteAllProducers(ProducerConsumerManager *manager);
#endif
