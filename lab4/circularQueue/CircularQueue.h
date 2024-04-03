#ifndef CIRCULAR_QUEUE_H
#define CIRCULAR_QUEUE_H

#include "../message/Message.h"

typedef struct {
  Message *messages;
  char *headPosition;
  char *tailPosition;
  int maxCapacity;
  int countAddedMessages;
  int countRemovedMessages;
  int currentSize;
} CircularQueue;

void addMessageToQueue(CircularQueue *queue, Message *message);
Message *removeMessageFromQueue(CircularQueue *queue);
int isQueueEmpty(CircularQueue *queue);
int isQueueFull(CircularQueue *queue);
void initializeQueue(CircularQueue *queue);
void printLastMessage(CircularQueue *queue);

#endif
