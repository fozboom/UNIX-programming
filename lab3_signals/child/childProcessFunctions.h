
#ifndef LAB3_CFUNCTIONS_H
#define LAB3_CFUNCTIONS_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <stdbool.h>
#define _XOPEN_SOURCE 700

#define COUNT_ALARMS_TO_PRINT_STATISTICS 118

struct Data {
    int firstNumber;
    int secondNumber;
};

struct Statistics {
    int countOfSituation_0_0;
    int countOfSituation_1_1;
    int countOfSituation_0_1;
    int countOfSituation_1_0;
};

void initializeTimer (struct itimerval *timer);
void alarmHandler ();
void requestParentPermissionToPrintStatistics(void);

void initializeParentHandler(void);


#endif //LAB3_CFUNCTIONS_H
