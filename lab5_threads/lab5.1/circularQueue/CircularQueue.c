#include "CircularQueue.h"

int isQueueFull(CircularQueue *queue) {
  return queue->currentSize == queue->maxCapacity;
}

int isQueueEmpty(CircularQueue *queue) { return queue->currentSize == 0; }

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
  if (isQueueFull(queue)) {
    printf("Queue is full\n");
    return;
  }
  queue->messages[queue->tailPosition] = message;
  queue->tailPosition = (queue->tailPosition + 1) % queue->maxCapacity;
  queue->currentSize++;
  queue->countAddedMessages++;
}

void removeMessageFromQueue(CircularQueue *queue) {
  if (isQueueEmpty(queue)) {
    printf("Queue is empty\n");
    return;
  }
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
