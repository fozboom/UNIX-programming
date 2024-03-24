#include "childProcessFunctions.h"

static const struct Data zero = {0, 0};
static const struct Data one = {1, 1};
int countAlarms = 0;
struct Data data;
struct Statistics statistics;
bool statistickOutputBlocked = false;

int main(void) {
  initializeParentHandler();

  struct itimerval timer;
  initializeTimer(&timer);
  while (1) {
    data = zero;
    data = one;

    if (countAlarms == COUNT_ALARMS_TO_PRINT_STATISTICS) {
      if (statistickOutputBlocked == false) {
        printf("\nPPID: %5d, PID: %5d, 00: %5d, 11: %5d, 01: %5d, 10: %5d\n",
               getppid(), getpid(), statistics.countOfSituation_0_0,
               statistics.countOfSituation_1_1, statistics.countOfSituation_0_1,
               statistics.countOfSituation_1_0);
      }
      countAlarms = 0;
    }
  }
}
