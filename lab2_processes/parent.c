#include "pFunctions.h"

extern char **environ;

#define CHILD_PATH "CHILD_PATH"
#define CHILD_PROGRAM_NAME_FORMAT "child_%02zu"
#define OPTION_GETENV '+'
#define OPTION_ENVP '*'
#define OPTION_ENVIRON '&'
#define OPTION_EXIT 'q'

void printMenu() 
{
    printf(PURPLE_COLOR);
    printf("\n------------------------------------");
    printf( "  \n|'+' - child process with getenv() |\n"
                "|'*' - child process with envp[]   |\n"
                "|'&' - child process with environ  |\n"
                "|'q' - exit                        |\n");
    printf("------------------------------------\n> ");
    printf(DEFAULT_COLOR);
}

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

        if (pid == 0)
        {
            printf("\nChild process created.");
            execve(childPath, childArgvParametrs, envp);
        }

        int processStatus;
        wait(&processStatus);
    }

    return 0;
}