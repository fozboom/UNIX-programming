#include "CircularQueue.h"
#include <stdlib.h>

int isQueueFull(CircularQueue *queue) {
  return queue->currentSize == queue->maxCapacity;
}

int isQueueEmpty(CircularQueue *queue) { return queue->currentSize == 0; }

void initializeQueue(CircularQueue *queue, int capacity) {
  queue->messages = (Message **)malloc(capacity * sizeof(Message));
  queue->maxCapacity = capacity;
  queue->currentSize = 0;
  queue->headIndex = 0;
  queue->tailIndex = 0;
  queue->countAddedMessages = 0;
  queue->countRemovedMessages = 0;
}

void addMessageToQueue(CircularQueue *queue, Message *message) {
  if (isQueueFull(queue)) {
    printf("Queue is full\n");
    return;
  }
  queue->messages[queue->tailIndex] = message;
  queue->tailIndex = (queue->tailIndex + 1) % queue->maxCapacity;
  queue->currentSize++;
}

Message *removeMessageFromQueue(CircularQueue *queue) {
  if (isQueueEmpty(queue)) {
    printf("Queue is empty\n");
    return NULL;
  }
  Message *message = queue->messages[queue->headIndex];
  queue->headIndex = (queue->headIndex + 1) % queue->maxCapacity;
  queue->currentSize--;
  return message;
}
