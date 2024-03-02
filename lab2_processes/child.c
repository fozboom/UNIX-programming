#include "cFunctions.h"



int main(int argc, char* argv[], char* envp[]) 
{
    // for (char** env = environ; *env != 0; env++)
    // {
    //     char* thisEnv = *env;
    //     printf("%s\n", thisEnv);
    // }
    printProcessInfo(argv[0]);
    handleFile(argv[1], envp, argv[2][0]);

    return 0;
}