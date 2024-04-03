#include "Message.h"
#include "../projectUtils/Utils.h"

Message *createMessage() {
  int shmid = shm_open(SHARED_MEMORY_NAME, O_CREAT | O_RDWR, 0666);
  if (shmid == -1) {
    perror("shm_open");
    exit(EXIT_FAILURE);
  }

  Message *message = mmap(NULL, sizeof(Message), PROT_READ | PROT_WRITE,
                          MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  if (message == MAP_FAILED) {
    perror("mmap");
    exit(EXIT_FAILURE);
  }
  do {
    message->size = rand() % 257;
  } while (message->size == 0);

  for (size_t i = 0; i < message->size; i++) {
    int is_upper_case = rand() % 2;
    if (is_upper_case) {
      message->data[i] = 'A' + rand() % 26;
    } else {
      message->data[i] = 'a' + rand() % 26;
    }
  }
  message->data[message->size] = '\0';
  close(shmid);
  return message;
}

void printMessage(Message *message) {
  if (message == NULL) {
    printf("Message is NULL\n");
    return;
  }
  printf("Message size: %d\n", message->size);
  printf("Message data: ");
  for (size_t i = 0; i < message->size; i++) {
    printf("%c", message->data[i]);
  }
  printf("\n");
}
