#include "pFunctions.h"



extern char **environ;



int main(int argc, char* argv[], char* envp[]) 
{
    printSortingEnviron(environ);

    size_t countChildProcesses = 0;
    const char* childPathParametr = "CHILD_PATH";

    while (1) 
    {
        printf("\033[0;35m");
        printf("\n------------------------------------");
        printf( "  \n|'+' - child process with getenv() |\n"
                    "|'*' - child process with envp[]   |\n"
                    "|'&' - child process with environ  |\n"
                    "|'q' - exit                        |\n");
        printf("------------------------------------\n> ");
        int option = getchar();
        getchar();

        printf("\033[0m"); 
        if (option == 'q') 
        {
            exit(EXIT_SUCCESS);
        }
        if (option != '+' && option != '*' && option != '&') 
        {
            continue;
        }


        char* childPath = NULL;
        switch (option)
        {
        case '+':
            childPath = getenv(childPathParametr);
            break;
        case '*':
            childPath = parsingEnviron(envp, childPathParametr);
            break;
        case '&':
            childPath = parsingEnviron(environ, childPathParametr);    
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
        sprintf(childProgramName, "child_%02zu", countChildProcesses++);

        char method[2] = {option, '\0'};
        char* childArgvParametrs[] = {childProgramName, argv[1],method, NULL};

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
