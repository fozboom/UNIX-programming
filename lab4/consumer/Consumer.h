#include "../circularQueue/CircularQueue.h"
#include "../message/Message.h"
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_COUNT_OF_CONSUMERS 15

extern pid_t consumersPid[];
extern size_t consumersCount;
extern volatile sig_atomic_t keepRunningConsumer;
extern sem_t emptySlotsSemaphore;
extern sem_t usedSlotsSemaphore;
extern sem_t queueMutex;

void createConsumer();
void deleteConsumer();
void deleteAllConsumers();
