#include "projectUtils/Utils.h"
#include <semaphore.h>

int main() {

  int sharedMemoryId;
  CircularQueue *queue;
  sem_t *emptySlotsSemaphore;
  sem_t *usedSlotsSemaphore;

  initializeSharedMemory(&sharedMemoryId, &queue);

  initializeQueue(queue);
  initializeHandler();

  handleInput(sharedMemoryId, queue);
  cleanResources();
}