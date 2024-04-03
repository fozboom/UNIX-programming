#include "CircularQueue.h"
#include "../projectUtils/Utils.h"
#include <stdlib.h>
#include <sys/mman.h>

int isQueueFull(CircularQueue *queue) {
  return queue->currentSize == queue->maxCapacity;
}

int isQueueEmpty(CircularQueue *queue) { return queue->currentSize == 0; }

void initializeQueue(CircularQueue *queue) {
  int memoryDescriptor = shm_open(SHARED_MEMORY_NAME, O_CREAT | O_RDWR, 0666);
  if (memoryDescriptor == -1) {
    perror("shm_open");
    exit(EXIT_FAILURE);
  }

  queue->headPosition = (char *)mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE,
                                     MAP_SHARED, memoryDescriptor, 0) +
                        sizeof(CircularQueue);
  if (queue->headPosition == MAP_FAILED) {
    perror("mmap");
    exit(EXIT_FAILURE);
  }
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

  // Копирование структуры Message в общую память
  memcpy(queue->tailPosition, message, sizeof(Message));
  queue->tailPosition += sizeof(Message);

  // Копирование данных сообщения в общую память
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

  // Чтение сообщения из общей памяти
  Message *message = (Message *)queue->headPosition;

  // Создание копии сообщения для возврата
  Message *messageCopy = (Message *)malloc(sizeof(Message));
  messageCopy->type = message->type;
  messageCopy->hash = message->hash;
  messageCopy->size = message->size;
  messageCopy->data = (char *)malloc(message->size);
  memcpy(messageCopy->data, queue->headPosition + sizeof(Message),
         message->size);

  // Обновление headPosition
  queue->headPosition += sizeof(Message) + message->size;

  queue->currentSize--;

  return messageCopy;
}
