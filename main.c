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


void walkInDirectories (char * path, bool symbolicReference, bool directory,
                            _Bool files, bool sorting)
{
    DIR * dirPointer = opendir(path);
    struct dirent * readFile;
    struct stat fileObject;
    while(1)
    {
        readFile = readdir(dirPointer);

        if(NULL == readFile)
        {
            if (errno)
                printf("Error openin directory");
            break;
        }

        if (strcmp(".", readFile->d_name) == 0 || strcmp("..", readFile->d_name) == 0)
        {
            continue;
        }
        
        printf("\n%s\n", readFile->d_name);
    
    }
    closedir(dirPointer);

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

    


    printf ("%d %d %d %d  ------%s \n", symbolicReference, directory, files, sorting, path);

    walkInDirectories(path, symbolicReference, directory, files, sorting);


    return 0;
}

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <locale.h>
// #include <dirent.h>

// int compare(const struct dirent **a, const struct dirent **b) {
//     return strcoll((*a)->d_name, (*b)->d_name);
// }

// int main() {
//     setlocale(LC_COLLATE, ""); 
//     struct dirent **namelist;
//     int n;

//     // Сканируем текущий каталог и сохраняем результаты в namelist
//     n = scandir("/home/daniel", &namelist, NULL, compare);
//     if (n < 0) {
//         perror("scandir");
//         exit(EXIT_FAILURE);
//     }

//     // Выводим список элементов каталога
//     for (int i = 0; i < n; i++) {
//         printf("%s\n", namelist[i]->d_name);
//         free(namelist[i]); // Освобождаем память для каждого элемента
//     }
//     free(namelist); // Освобождаем память для массива указателей

//     return 0;
// }
