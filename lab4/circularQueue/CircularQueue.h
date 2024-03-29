#include "../message/Message.h"

typedef struct {
  Message **messages;
  int headIndex;
  int tailIndex;
  int currentSize;
  int maxCapacity;
  int countAddedMessages;
  int countRemovedMessages;
} CircularQueue;

void addMessageToQueue(CircularQueue *queue, Message *message);
Message *removeMessageFromQueue(CircularQueue *queue);
int isQueueEmpty(CircularQueue *queue);
int isQueueFull(CircularQueue *queue);
void initializeQueue(CircularQueue *queue, int maxCapacity);