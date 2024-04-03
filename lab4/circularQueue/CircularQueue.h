#ifndef CIRCULAR_QUEUE_H
#define CIRCULAR_QUEUE_H

#include "../message/Message.h"
#include "../projectUtils/Utils.h"

typedef struct {
  Message *messages;
  int headIndex;
  int tailIndex;
  int currentSize;
  int maxCapacity;
  int countAddedMessages;
  int countRemovedMessages;
} CircularQueue;

void addMessageToQueue(CircularQueue *queue, Message message);
void removeMessageFromQueue(CircularQueue *queue);
int isQueueEmpty(CircularQueue *queue);
int isQueueFull(CircularQueue *queue);
void initializeQueue(CircularQueue *queue, int maxCapacity);
void printLastMessage(CircularQueue *queue);

#endif
