#include <sys/types.h>                  
#include <dirent.h>                             //for DIR
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>                              //for perrror
#include <stdbool.h>
#include <unistd.h>                             //for getopt() function and lstat()
#include <bits/getopt_core.h>                   //for opterr = 0
#include <getopt.h>
#include <string.h>
#include <sys/stat.h>                           //lstat()
#include <sys/sysmacros.h>                  

int compare(const struct dirent **a, const struct dirent **b) {
    return strcoll((*a)->d_name, (*b)->d_name);
}

int nonCompare (const struct dirent **a, const struct dirent **b)
{
    return false;
}


void walkInDirectories(char *path, bool symbolicReference, bool directory,
                       bool files, bool sorting) 
{

    struct dirent ** catalog;
    size_t n = scandir(path, &catalog, NULL, sorting ? compare : nonCompare);

    for (size_t i = 0; i < n; i++)
    {
       struct dirent * readFile = catalog[i];
       struct stat fileObject;

       if (strcmp(readFile->d_name, ".") == 0 || strcmp(readFile->d_name, "..") == 0) {
            continue;
        }

        char fullPath[256];
        snprintf(fullPath, 256, "%s/%s", path, readFile->d_name);
    

        if (lstat(fullPath, &fileObject) == -1) {
            perror("lstat");
            continue;
        }

        

        if (S_ISDIR(fileObject.st_mode)) 
        {
            if (directory) 
            {
                printf("\nDirectory: %s", fullPath);
            }
            walkInDirectories(fullPath, symbolicReference, directory, files, sorting);
        } 
        else if (S_ISREG(fileObject.st_mode) && files) 
        {
            printf("\nFile     : %s", fullPath);
        }
        else if (S_ISLNK(fileObject.st_mode) && symbolicReference)
        {
            printf("\nSymbolic link: %s", fullPath);
        }
    }
    
}                      
                                

int main(int argc, char* argv[])
{
    char * path = ".";
    int option = 0;

    int symbolicReference = false;
    int directory = false;
    int files = false;
    int sorting = false;

    bool haveParametrs = false;

    int option_index = 0;

    static struct option long_options[]=
    {
        {"l", 0, NULL,'l'},
        {"d", 0, NULL, 'd'},
        {"f", 0, NULL, 'f'},
        {"s", 0, NULL, 's'},
        {0,0,0,0}
    };

    for(int i = 0; i < argc; ++i)
    {
        if (argv[i][0] != '-')
        {
            path = argv[i];
        }
    }

    opterr = 0;                                             //чтобы getopt не бросала ошибки
    while ((option = getopt_long(argc, argv, "ldfs", long_options, &option_index)) != -1)
    {
        haveParametrs = true;
        switch (option)
        {
        case 'l':
            symbolicReference = true;
            break;
        case 'd':
            directory = true;
            break;
        case 'f':
            files = true;
            break;
        case 's':
            sorting = true;
            break;      
        default:
            printf("Invalid parametrs\n");
            return -1;
            break;
        }
    }

    if (haveParametrs == false)
    {
        symbolicReference = true;
        directory = true;
        files = true;
    }

    walkInDirectories(path, symbolicReference, directory, files, sorting);

    return 0;
}

