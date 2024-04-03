#include "Message.h"
#include "../projectUtils/Utils.h"

Message createMessage() {
  Message message;
  do {
    message.size = rand() % 257;
  } while (message.size == 0);

  for (size_t i = 0; i < message.size; i++) {
    int is_upper_case = rand() % 2;
    if (is_upper_case) {
      message.data[i] = 'A' + rand() % 26;
    } else {
      message.data[i] = 'a' + rand() % 26;
    }
  }
  message.data[message.size] = '\0';
  return message;
}

void printMessage(Message message) {
  if (message.size == 0) {
    printf("Message is empty\n");
    return;
  }
  printf("Message size: %d\n", message.size);
  printf("Message data: ");
  for (size_t i = 0; i < message.size; i++) {
    printf("%c", message.data[i]);
  }
  printf("\n");
}
