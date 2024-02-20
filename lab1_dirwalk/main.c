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

int compare(const struct dirent **a, const struct dirent **b);
int nonCompare (const struct dirent **a, const struct dirent **b);
void walkInDirectories(char *path, bool symbolicReference, bool directory,
                       bool files, bool sorting); 

int main(int argc, char* argv[])
{
    char * path = "./";
    int option = 0;
    int countParams = 0;

    bool symbolicReference = false;
    bool directory = false;
    bool files = false;
    bool sorting = false;

    bool haveParametrs = false;


    static struct option long_options[]=
    {
        {"l", 0, NULL,'l'},
        {"d", 0, NULL, 'd'},
        {"f", 0, NULL, 'f'},
        {"s", 0, NULL, 's'},
        {0,0,0,0}
    };

    for(int i = 1; i < argc; ++i)
    {
        if (argv[i][0] != '-')
        {
            path = argv[i];
        }
    }

    opterr = 0;                                             //чтобы getopt не бросала ошибки
    while ((option = getopt_long(argc, argv, "ldfs", long_options, NULL)) != -1)
    {
        haveParametrs = true;
        switch (option)
        {
        case 'l':
            symbolicReference = true;
            countParams++;
            break;
        case 'd':
            directory = true;
            countParams++;
            break;
        case 'f':
            files = true;
            countParams++;
            break;
        case 's':
            sorting = true;
            countParams++;
            break;      
        default:
            printf("Invalid parametrs\n");
            return -1;
            break;
        }
    }


    DIR * dir = opendir(path);
    if (!dir)
    {
        perror("\nNo such directory\n");
        return -1;
    }
    closedir(dir);


    printf("\n jfdkfjdkj %d fjkdljflkjd %d", countParams, symbolicReference);

    if ((haveParametrs == false) || ((countParams == 1) && sorting))
    {
        symbolicReference = true;
        directory = true;
        files = true;
    }

    walkInDirectories(path, symbolicReference, directory, files, sorting);

    printf("\n");

    return 0;
}

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
    char fullPath[256];
    for (size_t i = 0; i < n; i++)
    {
       struct dirent * readFile = catalog[i];
       struct stat fileObject;

       if (strcmp(readFile->d_name, ".") == 0 || strcmp(readFile->d_name, "..") == 0) {
            continue;
        }

        
        snprintf(fullPath, 256, "%s/%s", path, readFile->d_name);
    

        if (lstat(fullPath, &fileObject) == -1) {
            perror("lstat");
            continue;
        }

        

        if (S_ISDIR(fileObject.st_mode)) 
        {
            if (directory) 
            {
                printf("\nDirectory     : %s", fullPath);
            }
            walkInDirectories(fullPath, symbolicReference, directory, files, sorting);
        } 
        else if (S_ISREG(fileObject.st_mode) && files) 
        {
            printf("\nFile          : %s", fullPath);
        }
        else if (S_ISLNK(fileObject.st_mode) && symbolicReference)
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

