#include "pFunctions.h"

int comparator(const void *str1, const void *str2)
{
    return strcmp(*(const char **)str1, *(const char **)str2);
}



void printSortingEnviron (char* environ[])
{
    size_t countEnvironStrings = 0;

    while (environ[countEnvironStrings] != NULL)
    {
      countEnvironStrings++;
    }

    qsort(environ, countEnvironStrings, sizeof(char*), comparator);

    printf("\033[0;34m");  //blue


    for (size_t i = 0; i < countEnvironStrings; i++)
    {
        printf("%s\n", environ[i]);
    }
    

    printf("\033[0m");     //default
    
}


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