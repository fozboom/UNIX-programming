#include "functions.h"

int main(int argc, char* argv[])
{
    char * path = "./";
    int option = 0;
    int pathCount = 0;

    bool showSymLinks = false;
    bool showDirs = false;
    bool showFiles = false;
    bool sort = false;

    static struct option long_options[]=
    {
        {"l", 0, NULL,'l'},
        {"d", 0, NULL, 'd'},
        {"f", 0, NULL, 'f'},
        {"s", 0, NULL, 's'},
        {0,0,0,0}
    };

    while ((option = getopt_long(argc, argv, "ldfs", long_options, NULL)) != -1)
    {
        switch (option)
        {
        case 'l':
            showSymLinks = true;
            break;
        case 'd':
            showDirs = true;
            break;
        case 'f':
            showFiles = true;
            break;
        case 's':
            sort = true;
            break;      
        default:
            printf("Invalid parameters\n");
            return -1;
        }
    }

    for(int i = optind; i < argc; ++i)
    {
        pathCount++;
        path = argv[i];
    }

    if (pathCount > 1)
    {
        printf("\nPlease provide only one path\n");
        return -1;
    }

    DIR * dir = opendir(path);
    if (!dir)
    {
        printf("\nNo such directory\n");
        return -1;
    }
    closedir(dir);

    if (!showSymLinks && !showDirs && !showFiles)
    {
        showSymLinks = true;
        showDirs = true;
        showFiles = true;
        sort = true;
    }

    walkInDirectory(path, showSymLinks, showDirs, showFiles, sort);

    printf("\n");

    return 0;
}


              