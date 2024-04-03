#include "../consumer/Consumer.h"
#include "../producer/Producer.h"
#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <unistd.h>

#define QUEUE_SIZE 10
#define _XOPEN_SOURCE 700
#define SHM_KEY 1234
#define SHM_SIZE (sizeof(CircularQueue) + QUEUE_SIZE * (sizeof(Message) + 256))
#define SHARED_MEMORY_NAME "shared_memory"

extern sem_t emptySlotsSemaphore;
extern sem_t usedSlotsSemaphore;
extern sem_t queueMutex;
extern volatile sig_atomic_t keepRunningProducer;
extern volatile sig_atomic_t keepRunningConsumer;

void initializeSemaphorsAndMutexes();
void initializeHandler();
void printMenu();