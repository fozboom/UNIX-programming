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

#define MAX_THREADS 64
#define MIN_THREADS 8
#define RECORD_SIZE 4096
#define COUNT_CORES 8

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
extern int sort_record_count;
extern int sort_block_count;
extern int threads_count;
extern char *filename;

void check_arguments(int argc, char *argv[]);
void read_data_from_file(const char *filename);
void initialize_mutex_and_barrier();
void *map_file_and_launch_threads(void *data);
int compare_index_records(const void *a, const void *b);
void *sort_and_merge_block(void *data);
void sort_individual_block(thread_data *args);
void merge_sorted_blocks(thread_data *args);
void merge_individual_blocks(thread_data *args, int count_blocks_to_merge);