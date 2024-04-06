#ifndef UTILS_H
#define UTILS_H

#include "../circularQueue/CircularQueue.h"
#include "../consumer/Consumer.h"
#include "../defines.h"
#include "../producer/Producer.h"
#include "ProducerConsumerManager.h"
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>

extern volatile sig_atomic_t keepRunningProducer;
extern volatile sig_atomic_t keepRunningConsumer;

void initializeProducerConsumerManager(ProducerConsumerManager *manager,
                                       int maxProducers, int maxConsumers);
void printMenu();
void handleInput(ProducerConsumerManager *manager);
void initializeSemaphoresAndMutex(ProducerConsumerManager *manager);
void destroySemaphoresAndMutex(ProducerConsumerManager *manager);
void freeProducerConsumerManager(ProducerConsumerManager *manager);

#endif