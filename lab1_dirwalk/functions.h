#include <sys/types.h>                  
#include <dirent.h>                             
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>                              
#include <stdbool.h>
#include <unistd.h>                             
#include <bits/getopt_core.h>                   
#include <getopt.h>
#include <string.h>
#include <sys/stat.h>                           
#include <sys/sysmacros.h>        

int compareFileNames(const struct dirent **a, const struct dirent **b);
int noCompare(const struct dirent **a, const struct dirent **b);
void walkInDirectory(char *path, bool followSymLinks, bool showDirs,
                       bool showFiles, bool sort); 