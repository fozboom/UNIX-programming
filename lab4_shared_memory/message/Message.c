#include "Message.h"
#include "../projectUtils/Utils.h"

Message *createMessage() {
  size_t size;
  do {
    size = rand() % 257;
  } while (size == 0);

  Message *message = (Message *)malloc(sizeof(Message));

  message->size = size;
  message->data = (char *)malloc(size * sizeof(char));

  for (size_t i = 0; i < message->size; i++) {
    int is_upper_case = rand() % 2;
    if (is_upper_case) {
      message->data[i] = 'A' + rand() % 26;
    } else {
      message->data[i] = 'a' + rand() % 26;
    }
  }

  message->hash = 1;
  message->type = 0;
  return message;
}

void deleteMessage(Message *message) {
  if (message == NULL) {
    return;
  }
  free(message->data);
  free(message);
}

void printMessage(Message *message) {
  printf("Message Type: %u\n", message->type);
  printf("Message Hash: %u\n", message->hash);
  printf("Message Size: %u\n", message->size);
  printf("Message Data: ");
  for (size_t i = 0; i < message->size; i++) {
    printf("%c", message->data[i]);
  }
  printf("\n");
}
