#ifndef CIRCULAR_QUEUE_H
#define CIRCULAR_QUEUE_H

#include "../defines.h"
#include "../message/Message.h"
#include <pthread.h>
#include <stdbool.h>
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
  pthread_cond_t canRead;
  pthread_cond_t canWrite;
} CircularQueue;
void addMessageToQueue(CircularQueue *queue, Message *message);
void removeMessageFromQueue(CircularQueue *queue);
void initializeQueue(CircularQueue *queue);
void freeQueue(CircularQueue *queue);
bool increaseQueueSize(CircularQueue *queue);
bool decreaseQueueSize(CircularQueue *queue);
bool isQueueFull(CircularQueue *queue);
bool isQueueEmpty(CircularQueue *queue);

#endif
