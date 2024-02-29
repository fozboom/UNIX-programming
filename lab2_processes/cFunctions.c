#include "cFunctions.h"


void printProcessInfo(char* processName)
{
    printf(GREEN_COLOR);
    printf("------------------------");
    printf("\nName - %s", processName);
    printf("\nPid - %d", (int)getpid());
    printf("\nPpid - %d\n", (int)getppid());
    printf("------------------------");
    printf(DEFAULT_COLOR);
}

void handleFile(char* fileName, char* envp[], char option)
{
    FILE* file = fopen(fileName, "r");
    if (file == NULL)
    {
        printf("\nError opening file");
        exit(EXIT_FAILURE);
    }
    
    char buffer[BUFFER_SIZE];
    char* stringPointer = NULL;
    
    while (fgets(buffer, sizeof(buffer), file))
    {
        buffer[strcspn(buffer, "\n")] = '\0';
        
        switch (option)
        {
        case OPTION_GETENV:
            stringPointer = getenv(buffer);
            break;
        case OPTION_ENVP:
            stringPointer = parsingEnviron(envp, buffer);
            break;
        case OPTION_ENVIRON:
            stringPointer = parsingEnviron(environ, buffer);    
        default:
            break;
        }

        printf("\n%s=%s", buffer, stringPointer);
    }
    fclose(file);
}