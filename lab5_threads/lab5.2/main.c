#include "projectUtils/Utils.h"
#include <semaphore.h>

int main() {

  int sharedMemoryId;
  CircularQueue *queue;
  sem_t *emptySlotsSemaphore;
  sem_t *usedSlotsSemaphore;
  sem_t *queueMutex;

  initializeSharedMemory(&sharedMemoryId, &queue);
  initializeSemaphores(&emptySlotsSemaphore, &usedSlotsSemaphore, &queueMutex);

  initializeQueue(queue);
  initializeHandler();

  handleInput(sharedMemoryId, queue);
  cleanResources();
  closeSemaphores(emptySlotsSemaphore, usedSlotsSemaphore, queueMutex);
}