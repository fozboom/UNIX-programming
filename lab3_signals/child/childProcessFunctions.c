#include "childProcessFunctions.h"
#include <bits/types/siginfo_t.h>

extern int countAlarms;
extern struct Data data;
extern struct Statistics statistics;
extern bool statistickOutputBlocked;

void initializeTimer(struct itimerval *timer) {
  timer->it_interval.tv_sec = 0;
  timer->it_interval.tv_usec = 18181;
  timer->it_value = timer->it_interval;
  if (setitimer(ITIMER_REAL, timer, NULL)) {
    perror("setitimer");
    exit(errno);
  }
}

void alarmHandler() {
  if (data.firstNumber == 0 && data.secondNumber == 0) {
    statistics.countOfSituation_0_0++;
  } else if (data.firstNumber == 1 && data.secondNumber == 1) {
    statistics.countOfSituation_1_1++;
  } else if (data.firstNumber == 0 && data.secondNumber == 1) {
    statistics.countOfSituation_0_1++;
  } else if (data.firstNumber == 1 && data.secondNumber == 0) {
    statistics.countOfSituation_1_0++;
  }

  countAlarms++;
}

void blockStatisticsOutputHandler(int signal) {
  statistickOutputBlocked = true;
}

void allowStatisticsOutputHandler(int signal) {
  statistickOutputBlocked = false;
}

void parentHandler(int signal, siginfo_t *siginfo, void *context) {
  if (signal == SIGUSR1) {
    statistickOutputBlocked = true;
  } else if (signal == SIGUSR2) {
    statistickOutputBlocked = false;
  } else if (signal == SIGALRM) {
    alarmHandler();
  } else if (signal == SIGCHLD) {
    while (waitpid(-1, NULL, WNOHANG) > 0) {
    }
  } else if (signal == SIGSEGV) {
    printf("Segmentation fault\n");
    kill(getpid(), SIGKILL);
    exit(EXIT_FAILURE);
  }
}

void initializeParentHandler(void) {

  struct sigaction sa;
  sa.sa_flags = SA_SIGINFO;
  sa.sa_sigaction = parentHandler;
  sigemptyset(&sa.sa_mask);
  sigaction(SIGUSR1, &sa, NULL);
  sigaction(SIGUSR2, &sa, NULL);
  sigaction(SIGALRM, &sa, NULL);
  sigaction(SIGSEGV, &sa, NULL);
  sa.sa_sigaction = parentHandler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
  sigaction(SIGCHLD, &sa, 0);
}
