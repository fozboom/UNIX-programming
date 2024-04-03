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
#define SHM_SIZE (sizeof(CircularQueue) + QUEUE_SIZE * (sizeof(Message) + 256))
#define SHARED_MEMORY_NAME "shared_memory"
#define SEM_EMPTY_SLOTS "/sem-empty-slots"
#define SEM_USED_SLOTS "/sem-used-slots"
#define MUTEX "/mutex"
#define GREEN_COLOR "\033[0;32m"
#define RED_COLOR "\033[0;31m"
#define YELLOW_COLOR "\033[0;33m"
#define STANDART_COLOR "\033[0m"

extern volatile sig_atomic_t keepRunningProducer;
extern volatile sig_atomic_t keepRunningConsumer;

void initializeSemaphorsAndMutexes();
void initializeHandler();
void printMenu();
void initialize(int sharedMemoryId, CircularQueue *queue);
void handleInput(int sharedMemoryDescriptor, CircularQueue *queue);
void initializeSemaphores(sem_t **emptySlotsSemaphore,
                          sem_t **usedSlotsSemaphore, sem_t **queueMutex);
void initializeSharedMemory(int *sharedMemoryId, CircularQueue **queue);
void cleanResources();
void closeSemaphores(sem_t *emptySlotsSemaphore, sem_t *usedSlotsSemaphore,
                     sem_t *queueMutex);