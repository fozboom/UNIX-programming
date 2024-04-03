#ifndef PRODUCER_H
#define PRODUCER_H
#include "../circularQueue/CircularQueue.h"
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define _XOPEN_SOURCE 700
#define MAX_COUNT_OF_PRODUCERS 5

extern volatile sig_atomic_t keepRunningProducer;
extern pid_t producersPid[];
extern size_t producersCount;

void createProducer();
void deleteProducer();
void deleteAllProducers();
#endif
