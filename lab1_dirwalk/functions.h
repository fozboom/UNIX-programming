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
void parseOptions (int argc, char* argv[], bool* showSymLinks, bool* showDirs, bool* showFiles, bool* sort);
void checkDirectoryIsOpen (char* path);
void checkSinglePathProvided (int argc, char* argv[], char** path);
void setDefaultFlagsIfNoOptions (bool* showSymLinks, bool* showDirs, bool* showFiles, bool* sort);
void walkInDirectory(char *path, bool followSymLinks, bool showDirs, bool showFiles, bool sort); 
void checkFileStat (char* fullPath, bool followSymLinks, bool showDirs, bool showFiles, bool sort);