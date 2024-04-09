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

void increaseQueueSize(CircularQueue *queue) {
  int newCapacity = queue->maxCapacity + 1;
  queue->messages = realloc(queue->messages, newCapacity * sizeof(Message *));
  if (queue->headPosition > queue->tailPosition) {
    memmove(&queue->messages[queue->maxCapacity], &queue->messages[0],
            queue->tailPosition * sizeof(Message *));
    queue->tailPosition += queue->maxCapacity;
  }
  queue->maxCapacity = newCapacity;
}

void decreaseQueueSize(CircularQueue *queue) {
  if (queue->maxCapacity > 1 && queue->currentSize < queue->maxCapacity - 1) {
    int newCapacity = queue->maxCapacity - 1;
    if (queue->headPosition > queue->tailPosition) {
      if (queue->tailPosition < newCapacity - queue->maxCapacity) {
        memmove(&queue->messages[queue->maxCapacity - queue->tailPosition],
                &queue->messages[0], queue->tailPosition * sizeof(Message *));
        queue->headPosition -= queue->tailPosition;
      } else {
        memmove(&queue->messages[0], &queue->messages[queue->maxCapacity],
                (newCapacity - queue->headPosition) * sizeof(Message *));
        queue->tailPosition -= queue->headPosition;
      }
    }
    queue->messages = realloc(queue->messages, newCapacity * sizeof(Message *));
    queue->messages[queue->maxCapacity] = NULL;
    if (queue->messages == NULL) {
      printf("Memory reallocation failed\n");
      return;
    }
    queue->maxCapacity = newCapacity;
  } else {
    printf("Queue size can't be decreased\n");
  }
}
