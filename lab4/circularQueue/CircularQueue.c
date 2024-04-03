#include "CircularQueue.h"
#include <stdlib.h>

int isQueueFull(CircularQueue *queue) {
  return queue->currentSize == queue->maxCapacity;
}

int isQueueEmpty(CircularQueue *queue) { return queue->currentSize == 0; }

void initializeQueue(CircularQueue *queue, int capacity) {
  queue->messages = (Message *)malloc(capacity * sizeof(Message));
  queue->maxCapacity = capacity;
  queue->currentSize = 0;
  queue->headIndex = 0;
  queue->tailIndex = 0;
  queue->countAddedMessages = 0;
  queue->countRemovedMessages = 0;
}

void addMessageToQueue(CircularQueue *queue, Message message) {
  if (isQueueFull(queue)) {
    printf("Queue is full\n");
    return;
  }

  queue->messages[queue->tailIndex].size = message.size;
  queue->messages[queue->tailIndex].type = message.type;
  queue->messages[queue->tailIndex].hash = message.hash;
  printf("THIS FROM QUEUE\n");
  printf("Tail index - %d\n", queue->tailIndex);
  for (size_t i = 0; i < message.size; i++) {
    queue->messages[queue->tailIndex].data[i] = message.data[i];

    printf("%c", queue->messages[queue->tailIndex].data[i]);
  }
  queue->messages[queue->tailIndex].data[message.size] = '\0';
  printf("THIS FROM QUEUE");
  printMessage(queue->messages[queue->tailIndex]);

  queue->tailIndex = (queue->tailIndex + 1) % queue->maxCapacity;
  queue->currentSize++;
  queue->countAddedMessages++;
}

void removeMessageFromQueue(CircularQueue *queue) {
  printf("Current size before delete - %d\n", queue->currentSize);
  printf("Message size before delete - %d\n",
         queue->messages[queue->headIndex].size);
  if (isQueueEmpty(queue)) {
    printf("Queue is empty\n");
    return;
  }
  Message message = queue->messages[queue->headIndex];
  queue->headIndex = (queue->headIndex + 1) % queue->maxCapacity;
  queue->currentSize--;
  printf("THIS FROM QUEUE");
  printMessage(message);
  printf("Good returning to queue\n");
}

void printLastMessage(CircularQueue *queue) {
  if (isQueueEmpty(queue)) {
    printf("Queue is empty\n");
    return;
  }
  Message message =
      queue->messages[(queue->tailIndex - 1) % queue->maxCapacity];
  printMessage(message);
}
