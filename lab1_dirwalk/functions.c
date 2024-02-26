#include "functions.h"

int compareFileNames(const struct dirent **a, const struct dirent **b) {
    return strcoll((*a)->d_name, (*b)->d_name);
}

int noCompare(const struct dirent **a, const struct dirent **b)
{
    return false;
}


void walkInDirectory(char *path, bool followSymLinks, bool showDirs,
                       bool showFiles, bool sort) 
{

    struct dirent ** catalog;
    size_t n = scandir(path, &catalog, NULL, sort ? compareFileNames : noCompare);
    if (n < 0) 
    {
        perror("Scandir error");
        return;
    }
    char fullPath[256];
    for (size_t i = 0; i < n; i++)
    {
       struct dirent * readFile = catalog[i];
       struct stat fileStat;

       if (strcmp(readFile->d_name, ".") == 0 || strcmp(readFile->d_name, "..") == 0)
        {
            continue;
        }

        snprintf(fullPath, 256, "%s/%s", path, readFile->d_name);
    

        if (lstat(fullPath, &fileStat) == -1) 
        {
            perror("Lstat error");
            continue;
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
    
    for (size_t i = 0; i < n; ++i)
    {
        free(catalog[i]);
    }
    free(catalog);
}  