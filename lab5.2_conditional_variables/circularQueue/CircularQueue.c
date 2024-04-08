#include "CircularQueue.h"

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
  pthread_mutex_init(&queue->mutex, NULL);
  pthread_cond_init(&queue->notFull, NULL);
  pthread_cond_init(&queue->notEmpty, NULL);
}

void addMessageToQueue(CircularQueue *queue, Message *message) {
  pthread_mutex_lock(&queue->mutex);
  while (queue->currentSize == queue->maxCapacity) {
    pthread_cond_wait(&queue->notFull, &queue->mutex);
  }
  queue->messages[queue->tailPosition] = message;
  queue->tailPosition = (queue->tailPosition + 1) % queue->maxCapacity;
  queue->currentSize++;
  queue->countAddedMessages++;
  pthread_cond_signal(&queue->notEmpty);
  pthread_mutex_unlock(&queue->mutex);
}

void removeMessageFromQueue(CircularQueue *queue) {
  pthread_mutex_lock(&queue->mutex);
  while (queue->currentSize == 0) {
    pthread_cond_wait(&queue->notEmpty, &queue->mutex);
  }
  printMessage(queue->messages[queue->headPosition]);
  free(queue->messages[queue->headPosition]);
  queue->messages[queue->headPosition] = NULL;
  queue->headPosition = (queue->headPosition + 1) % queue->maxCapacity;
  queue->currentSize--;
  queue->countRemovedMessages++;
  pthread_cond_signal(&queue->notFull);
  pthread_mutex_unlock(&queue->mutex);
}

void freeQueue(CircularQueue *queue) {
  pthread_cond_destroy(&queue->notFull);
  pthread_cond_destroy(&queue->notEmpty);
  pthread_mutex_destroy(&queue->mutex);
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
  queue->messages = realloc(queue->messages, newCapacity * sizeof(Message *));
  if (queue->headPosition > queue->tailPosition) {
    memmove(&queue->messages[queue->maxCapacity], &queue->messages[0],
            queue->tailPosition * sizeof(Message *));
    queue->tailPosition += queue->maxCapacity;
  }
  queue->maxCapacity = newCapacity;
  pthread_cond_broadcast(&queue->notFull);
  pthread_mutex_unlock(&queue->mutex);
}

void decreaseQueueSize(CircularQueue *queue) {
  pthread_mutex_lock(&queue->mutex);
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
    queue->maxCapacity = newCapacity;
  } else {
    printf("Queue size can't be decreased\n");
  }
  pthread_mutex_unlock(&queue->mutex);
}
