#include "pFunctions.h"
#include <unistd.h>



int main(int argc, char* argv[], char* envp[]) 
{
    printSortingEnviron(environ);

    size_t childProcessesCount = 0;

    while (1) 
    {
        printMenu();
        int option = getchar();
        getchar();
        if (option == OPTION_EXIT) 
        {
            exit(EXIT_SUCCESS);
        }
        if (option != OPTION_GETENV && option != OPTION_ENVP && option != OPTION_ENVIRON) 
        {
            continue;
        }

        char* childPath = NULL;
        switch (option)
        {
        case OPTION_GETENV:
            childPath = getenv(CHILD_PATH);
            break;
        case OPTION_ENVP:
            childPath = parsingEnviron(envp, CHILD_PATH);
            break;
        case OPTION_ENVIRON:
            childPath = parsingEnviron(environ, CHILD_PATH); 
            break;   
        default:
            break;
        }
    

        pid_t pid = fork();

        if (pid == -1)
        {
            printf("Error occurred, error - %d\n", errno);
            exit(errno);
        }

        char childProgramName[9];
        sprintf(childProgramName, CHILD_PROGRAM_NAME_FORMAT, childProcessesCount++);

        char method[2] = {option, '\0'};
        char* childArgvParametrs[] = {childProgramName, argv[1], method, NULL};

        char** newEnviron = createReduceEnv(envp, argv[1]);
        if (pid == 0)
        {
            printf("\nChild process created.");
            execve(childPath, childArgvParametrs, newEnviron);
        }

        int processStatus;
        wait(&processStatus);
    }

    return 0;
}