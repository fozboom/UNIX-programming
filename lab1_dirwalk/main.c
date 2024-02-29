#include "functions.h"

int main(int argc, char* argv[])
{
    char * path = "./";

    bool showSymLinks = false;
    bool showDirs = false;
    bool showFiles = false;
    bool sort = false;

    parseOptions(argc, argv, &showSymLinks, &showDirs, &showFiles, &sort);

    checkSinglePathProvided(argc, argv, &path);

    checkDirectoryIsOpen(path);
    
    setDefaultFlagsIfNoOptions(&showSymLinks, &showDirs, &showFiles, &sort);

    walkInDirectory(path, showSymLinks, showDirs, showFiles, sort);

    printf("\n");

    return 0;
}


              