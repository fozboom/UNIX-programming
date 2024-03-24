#include "childProcessFunctions.h"

static const struct Data zero = {0, 0};
static const struct Data one = {1, 1};
int countAlarms = 0;
struct Data data;
struct Statistics statistics;
bool statistickOutputBlocked = false;

int main(int argc, char *argv[]) {
  initializeParentHandler();

  initializeTimer();
  while (1) {
    data = zero;
    data = one;

    if (countAlarms == COUNT_ALARMS_TO_PRINT_STATISTICS) {
      if (statistickOutputBlocked == false) {
        printf(
            "\n %s: PPID: %3d, PID: %3d, 00: %3d, 11: %3d, 01: %3d, 10: %5d\n",
            argv[0], getppid(), getpid(), statistics.countOfSituation_0_0,
            statistics.countOfSituation_1_1, statistics.countOfSituation_0_1,
            statistics.countOfSituation_1_0);
      }
      countAlarms = 0;
      resetStatistics();
    }
  }
}
