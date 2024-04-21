#include "CircularQueue.h"
#include <pthread.h>
#include <string.h>

bool isQueueFull(CircularQueue *queue) {
  return queue->currentSize == queue->maxCapacity;
}

bool isQueueEmpty(CircularQueue *queue) { return queue->currentSize == 0; }

void initializeQueue(CircularQueue *queue) {
  queue->messages = (Message **)calloc(QUEUE_SIZE, sizeof(Message *));
  for (int i = 0; i < QUEUE_SIZE; i++) {
    queue->messages[i] = NULL;
  }

  queue->tailPosition = queue->headPosition = 0;
  queue->maxCapacity = QUEUE_SIZE;
  queue->currentSize = 0;
  queue->countAddedMessages = 0;
  queue->countRemovedMessages = 0;
}

void addMessageToQueue(CircularQueue *queue, Message *message) {
  queue->messages[queue->tailPosition] = message;
  queue->tailPosition = (queue->tailPosition + 1) % queue->maxCapacity;
  queue->currentSize++;
  queue->countAddedMessages++;
}

void removeMessageFromQueue(CircularQueue *queue) {
  printMessage(queue->messages[queue->headPosition]);
  free(queue->messages[queue->headPosition]);
  queue->messages[queue->headPosition] = NULL;
  queue->headPosition = (queue->headPosition + 1) % queue->maxCapacity;
  queue->currentSize--;
  queue->countRemovedMessages++;
}

void freeQueue(CircularQueue *queue) {
  for (int i = 0; i < queue->maxCapacity; i++) {
    if (queue->messages[i] != NULL) {
      free(queue->messages[i]);
    }
  }
  free(queue->messages);
}

bool increaseQueueSize(CircularQueue *queue) {
  int newSize = queue->maxCapacity + 1;

  Message **newMessages = (Message **)malloc(newSize * sizeof(Message *));
  if (newMessages == NULL) {
    return false;
  }

  for (int i = 0; i < queue->currentSize; i++) {
    newMessages[i] =
        queue->messages[(queue->headPosition + i) % queue->maxCapacity];
  }

  free(queue->messages);
  queue->messages = newMessages;
  queue->headPosition = 0;
  queue->tailPosition = queue->currentSize % newSize;
  queue->maxCapacity = newSize;
  printf("Queue size increased to %d\n", newSize);
  return true;
}

bool decreaseQueueSize(CircularQueue *queue) {
  int newSize = queue->maxCapacity - 1;
  if (isQueueFull(queue)) {
    printf("Queue is full\n");
    return false;
  }

  Message **newMessages = (Message **)malloc(newSize * sizeof(Message *));
  if (newMessages == NULL) {
    return false;
  }

  for (int i = 0; i < newSize; i++) {
    newMessages[i] =
        queue->messages[(queue->headPosition + i) % queue->maxCapacity];
  }

  free(queue->messages);
  queue->messages = newMessages;
  queue->headPosition = 0;
  queue->tailPosition = queue->currentSize % newSize;
  queue->maxCapacity = newSize;
  printf("Queue size decreased to %d\n", newSize);
  return true;
}