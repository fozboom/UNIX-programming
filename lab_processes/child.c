#include "pFunctions.h"

extern char **environ;


char* parsingEnviron (char * envp[], const char* parametrName)
{
    while (*envp != NULL)
    {
        if (strncmp(*envp, parametrName, strlen(parametrName)) == 0)
        {
            return *envp + strlen(parametrName) + 1;
        }


        ++envp;
    }
    
    return NULL;
}



int main(int argc, char* argv[], char* envp[]) 
{
    printf("\033[1;32m");   //green
    printf("------------------------");
    printf("\nName - %s", argv[0]);
    printf("\nPid - %d", (int)getpid());
    printf("\nPpid - %d\n", (int)getppid());
    printf("------------------------");
    printf("\033[0m");

    
    FILE* file = fopen(argv[1], "r");
    if (file == NULL)
    {
        printf("\nError opening file");
        exit(EXIT_FAILURE);
    }
    
    char buffer[80];
    char* stringPointer = NULL;
    
    while (fgets(buffer, sizeof(buffer), file))
    {
        
        buffer[strcspn(buffer, "\n")] = '\0';
        
        switch (argv[2][0])
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


    return 0;
}
