#ifndef CIRCULAR_QUEUE_H
#define CIRCULAR_QUEUE_H

#include "../defines.h"
#include "../message/Message.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  Message **messages;
  int headPosition;
  int tailPosition;
  int maxCapacity;
  int countAddedMessages;
  int countRemovedMessages;
  int currentSize;
} CircularQueue;

void addMessageToQueue(CircularQueue *queue, Message *message);
void removeMessageFromQueue(CircularQueue *queue);
int isQueueEmpty(CircularQueue *queue);
int isQueueFull(CircularQueue *queue);
void initializeQueue(CircularQueue *queue);
void printLastMessage(CircularQueue *queue);
void freeQueue(CircularQueue *queue);

#endif
