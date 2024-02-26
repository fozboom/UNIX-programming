#include "pFunctions.h"



extern char **environ;


char* parsingEnviron (char * envp[], const char* parameterName)
{
    while (*envp != NULL)
    {
        if (strncmp(*envp, parameterName, strlen(parameterName)) == 0)
        {
            return *envp + strlen(parameterName) + 1;
        }
        ++envp;
    }
    return NULL;
}

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

void handleFile(char* fileName, char* envp[])
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
        
        switch (fileName[0])
        {
        case '+':
            stringPointer = getenv(buffer);
            break;
        case '*':
            stringPointer = parsingEnviron(envp, buffer);
            break;
        case '&':
            stringPointer = parsingEnviron(environ, buffer);    
        default:
            break;
        }

        printf("\n%s=%s", buffer, stringPointer);
    }
    fclose(file);
}

int main(int argc, char* argv[], char* envp[]) 
{
    printProcessInfo(argv[0]);
    handleFile(argv[1], envp);

    return 0;
}