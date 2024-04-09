#ifndef CIRCULAR_QUEUE_H
#define CIRCULAR_QUEUE_H

#include "../defines.h"
#include "../message/Message.h"
#include <pthread.h>
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
  pthread_mutex_t mutex;
} CircularQueue;

void addMessageToQueue(CircularQueue *queue, Message *message);
void removeMessageFromQueue(CircularQueue *queue);
int isQueueEmpty(CircularQueue *queue);
int isQueueFull(CircularQueue *queue);
void initializeQueue(CircularQueue *queue);
void printLastMessage(CircularQueue *queue);
void freeQueue(CircularQueue *queue);
void increaseQueueSize(CircularQueue *queue);
void decreaseQueueSize(CircularQueue *queue);

#endif
