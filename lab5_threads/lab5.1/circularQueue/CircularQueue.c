#include "CircularQueue.h"
#include "../projectUtils/Utils.h"
#include <stdlib.h>
#include <sys/mman.h>

int isQueueFull(CircularQueue *queue) {
  return queue->currentSize == queue->maxCapacity;
}

int isQueueEmpty(CircularQueue *queue) { return queue->currentSize == 0; }

void initializeQueue(CircularQueue *queue) {

  queue->tailPosition = queue->headPosition;

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

  memcpy(queue->tailPosition, message, sizeof(Message));
  queue->tailPosition += sizeof(Message);

  memcpy(queue->tailPosition, message->data, message->size);
  queue->tailPosition += message->size;

  queue->currentSize++;
  queue->countAddedMessages++;
}

Message *removeMessageFromQueue(CircularQueue *queue) {
  if (isQueueEmpty(queue)) {
    printf("Queue is empty\n");
    return NULL;
  }

  Message *message = (Message *)queue->headPosition;

  Message *messageCopy = (Message *)malloc(sizeof(Message));
  messageCopy->type = message->type;
  messageCopy->hash = message->hash;
  messageCopy->size = message->size;
  messageCopy->data = (char *)malloc(message->size);
  memcpy(messageCopy->data, queue->headPosition + sizeof(Message),
         message->size);

  queue->headPosition += sizeof(Message) + message->size;

  queue->currentSize--;
  queue->countRemovedMessages++;

  return messageCopy;
}
