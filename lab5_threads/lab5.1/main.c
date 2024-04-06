
#include "projectUtils/ProducerConsumerManager.h"
#include "projectUtils/Utils.h"
#include <stdlib.h>

int main() {
  ProducerConsumerManager *manager =
      (ProducerConsumerManager *)malloc(sizeof(ProducerConsumerManager));
  initializeProducerConsumerManager(manager, MAX_COUNT_OF_PRODUCERS,
                                    MAX_COUNT_OF_CONSUMERS);

  handleInput(manager);
}