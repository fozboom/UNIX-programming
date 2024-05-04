#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <stdlib.h>
#include "index_data.h"
#include <threads.h>
#include <pthread.h>
#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

typedef struct
{
	int memsize;
	int blocks;
	int threads;
	const char *filename;
} Arguments;

typedef struct
{
	int thread_num;
	size_t block_size;
	index_record *buffer;
} thread_data;

extern pthread_mutex_t mutex;
extern pthread_barrier_t barrier;
extern index_hdr_s hdr;
extern index_record *currentBlock;
extern int size_to_sort;
extern int count_blocks;

Arguments check_arguments(int argc, char *argv[]);
void initialize_mutex_and_barrier(Arguments args);
void *map_file_and_launch_threads(void *data);
int compare(const void *a, const void *b);
void *sort_block(void *data);