#include "cFunctions.h"


int main(int argc, char* argv[], char* envp[]) 
{
    
    printProcessInfo(argv[0]);
    handleFile(argv[1], envp, argv[2][0]);

    return 0;
}