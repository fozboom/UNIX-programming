#include "CircularQueue.h"
#include <pthread.h>
#include <string.h>

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
  queue->messages[queue->tailPosition] = message;
  queue->tailPosition = (queue->tailPosition + 1) % queue->maxCapacity;
  queue->currentSize++;
  queue->countAddedMessages++;
}

void removeMessageFromQueue(CircularQueue *queue) {
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
  pthread_mutex_lock(&queue->mutex);
  int newCapacity = queue->maxCapacity + 1;
  Message **newMessages =
      realloc(queue->messages, newCapacity * sizeof(Message *));
  if (newMessages == NULL) {
    printf("Memory reallocation failed\n");
    pthread_mutex_unlock(&queue->mutex);
    return;
  }
  if (queue->headPosition < queue->tailPosition) {
    memmove(&newMessages[0], &queue->messages[queue->headPosition],
            (queue->maxCapacity - queue->headPosition) * sizeof(Message *));
    memmove(&newMessages[newCapacity - queue->headPosition],
            &queue->messages[0], queue->tailPosition * sizeof(Message *));
    queue->headPosition = 0;
    queue->tailPosition = queue->currentSize;
  } else {
    memmove(&newMessages[0], &queue->messages[queue->headPosition],
            queue->currentSize * sizeof(Message *));
    queue->headPosition = 0;
    queue->tailPosition = queue->currentSize;
  }

  queue->maxCapacity = newCapacity;
  queue->messages = newMessages;
  printf("Queue size increased\n");
  pthread_mutex_unlock(&queue->mutex);
}

void decreaseQueueSize(CircularQueue *queue) {
  pthread_mutex_lock(&queue->mutex);

  if (queue->currentSize < queue->maxCapacity) {
    int newCapacity = queue->maxCapacity - 1;
    Message **newMessages =
        realloc(queue->messages, newCapacity * sizeof(Message *));

    if (newMessages == NULL) {
      printf("Memory reallocation failed\n");
      pthread_mutex_unlock(&queue->mutex);
      return;
    }

    if (queue->headPosition > queue->tailPosition) {
      memmove(&newMessages[queue->headPosition - 1],
              &newMessages[queue->headPosition],
              (queue->maxCapacity - queue->headPosition) * sizeof(Message *));
      queue->headPosition--;
    }

    queue->messages = newMessages;
    queue->maxCapacity = newCapacity;
    printf("Queue size decreased\n");
  } else {
    printf("Cannot decrease size, queue is full\n");
  }

  pthread_mutex_unlock(&queue->mutex);
}
