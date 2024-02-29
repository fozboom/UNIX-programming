#include "functions.h"
#include <stdlib.h>

int compareFileNames(const struct dirent **a, const struct dirent **b) {
    return strcoll((*a)->d_name, (*b)->d_name);
}

int noCompare(const struct dirent **a, const struct dirent **b)
{
    return false;
}

void parseOptions (int argc, char* argv[], bool* showSymLinks, bool* showDirs, bool* showFiles, bool* sort)
{
    int option;
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
            *showSymLinks = true;
            break;
        case 'd':
            *showDirs = true;
            break;
        case 'f':
            *showFiles = true;
            break;
        case 's':
            *sort = true;
            break;      
        default:
            printf("Invalid parameters\n");
            exit(EXIT_FAILURE);
        }
    }
}


void checkSinglePathProvided (int argc, char* argv[], char** path)
{
    int pathCount = 0;
    for(int i = optind; i < argc; ++i)
    {
        pathCount++;
        *path = argv[i];
    }

    if (pathCount > 1)
    {
        printf("\nPlease provide only one path\n");
        exit(EXIT_FAILURE);
    }
}


void setDefaultFlagsIfNoOptions (bool* showSymLinks, bool* showDirs, bool* showFiles, bool* sort)
{
    if (!(*showSymLinks) && !(*showDirs) && !(*showFiles))
    {
        *showSymLinks = true;
        *showDirs = true;
        *showFiles = true;
        *sort = true;
    }
}

void checkDirectoryIsOpen (char* path)
{
    DIR * dir = opendir(path);
    if (!dir)
    {
        printf("\nNo such directory\n");
        exit(EXIT_FAILURE);
    }
    closedir(dir);
}

void checkFileStat (char* fullPath, bool followSymLinks, bool showDirs, bool showFiles, bool sort)
{
    struct stat fileStat;
    if (lstat(fullPath, &fileStat) == -1) 
    {
        perror("Lstat error");
        return;
    }

    if (S_ISDIR(fileStat.st_mode)) 
    {
        if (showDirs) 
        {
            printf("\nDirectory     : %s", fullPath);
        }
        walkInDirectory(fullPath, followSymLinks, showDirs, showFiles, sort);
    } 
    else if (S_ISREG(fileStat.st_mode) && showFiles) 
    {
        printf("\nFile          : %s", fullPath);
    }
    else if (S_ISLNK(fileStat.st_mode) && followSymLinks)
    {
        printf("\nSymbolic link : %s", fullPath);
    }
}



void walkInDirectory(char *path, bool followSymLinks, bool showDirs,
                       bool showFiles, bool sort) 
{

    struct dirent ** catalog;

    char fullPath[256];
    struct dirent * readFile;
    struct stat fileStat;

    size_t n = scandir(path, &catalog, NULL, sort ? compareFileNames : noCompare);
    if (n < 0) 
    {
        perror("Scandir error");
        return;
    }

    for (size_t i = 0; i < n; i++) {
        readFile = catalog[i];

        if (strcmp(readFile->d_name, ".") == 0 || strcmp(readFile->d_name, "..") == 0) {
            continue;
        }

        snprintf(fullPath, 256, "%s/%s", path, readFile->d_name);
    
        checkFileStat(fullPath, followSymLinks, showDirs, showFiles, sort);
    }
    
    for (size_t i = 0; i < n; ++i)
    {
        free(catalog[i]);
    }
    free(catalog);
}  

