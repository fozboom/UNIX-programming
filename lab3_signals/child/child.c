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
        printf("\033[1;32m\n %s: \033[0m" // Green color for program name
               "\033[1;34mPPID: \033[0m"  // Blue color for PPID
               "\033[1;35m%3d, \033[0m"   // Purple color for PPID value
               "\033[1;34mPID: \033[0m"   // Blue color for PID
               "\033[1;35m%3d, \033[0m"   // Purple color for PID value
               "\033[1;33m00: \033[0m"    // Yellow color for 00
               "\033[1;35m%3d, \033[0m"   // Purple color for 00 value
               "\033[1;33m11: \033[0m"    // Yellow color for 11
               "\033[1;35m%3d, \033[0m"   // Purple color for 11 value
               "\033[1;33m01: \033[0m"    // Yellow color for 01
               "\033[1;35m%3d, \033[0m"   // Purple color for 01 value
               "\033[1;33m10: \033[0m"    // Yellow color for 10
               "\033[1;35m%5d\033[0m\n",  // Purple color for 10 value
               argv[0], getppid(), getpid(), statistics.countOfSituation_0_0,
               statistics.countOfSituation_1_1, statistics.countOfSituation_0_1,
               statistics.countOfSituation_1_0);
      }
      countAlarms = 0;
      resetStatistics();
    }
  }
  countAlarms = 0;
  resetStatistics();
}
