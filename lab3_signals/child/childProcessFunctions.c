#include "childProcessFunctions.h"
#include <bits/types/siginfo_t.h>
#include <signal.h>

extern int countAlarms;
extern struct Data data;
extern struct Statistics statistics;
extern bool statistickOutputBlocked;

void initializeTimer(void) {
  struct itimerval timer;
  timer.it_interval.tv_sec = 0;
  timer.it_interval.tv_usec = 18181;
  timer.it_value = timer.it_interval;

  /*
    ITIMER_REAL - устанавливает таймер реального времени
    ITIMER_VIRTUAL - устанавливает таймер виртуального времени

  */
  if (setitimer(ITIMER_REAL, &timer, NULL)) {
    perror("setitimer");
    exit(errno);
  }
}

void alarmHandler(void) {
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
  }
}

void initializeParentHandler(void) {
  struct sigaction sa, oldsa;
  /*
    SA_SIGINFO - указывает, что sa_handler указывает на функцию обработчика с
    тремя аргументами (вместо одного аргумента, как в случае с SA_HANDLER)
    SA_RESTART - указывает, что системный вызов, прерванный сигналом, должен
    быть автоматически возобновлен
    SA_NOLCLDSTOP - указывает, что процесс-родитель не должен получать сигнал
    остановки
    SA_NOLDCLDWAIT - указывает, что ребенок сам удалится из зомби
  */
  sa.sa_flags = SA_SIGINFO | SA_RESTART | SA_NOCLDWAIT;
  sa.sa_sigaction = parentHandler;
  sigemptyset(&sa.sa_mask);

  sigaction(SIGUSR1, NULL, &oldsa);
  if (oldsa.sa_handler == SIG_IGN) {
    printf("SIGUSR1 is currently set to be ignored\n");
  }
  sigaction(SIGUSR1, &sa, NULL);

  sigaction(SIGUSR2, NULL, &oldsa);
  if (oldsa.sa_handler == SIG_IGN) {
    printf("SIGUSR2 is currently set to be ignored\n");
  }
  sigaction(SIGUSR2, &sa, NULL);

  sigaction(SIGALRM, NULL, &oldsa);
  if (oldsa.sa_handler == SIG_IGN) {
    printf("SIGALRM is currently set to be ignored\n");
  }
  sigaction(SIGALRM, &sa, NULL);

  sigaction(SIGCHLD, NULL, &oldsa);
  if (oldsa.sa_handler == SIG_IGN) {
    printf("SIGCHLD is currently set to be ignored\n");
  }
  sigaction(SIGCHLD, &sa, NULL);
}

void resetStatistics(void) {
  statistics.countOfSituation_0_0 = 0;
  statistics.countOfSituation_1_1 = 0;
  statistics.countOfSituation_0_1 = 0;
  statistics.countOfSituation_1_0 = 0;
}
