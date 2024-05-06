#include "sort_index_utils.h"

pthread_mutex_t mutex;
pthread_barrier_t barrier;
index_hdr_s hdr;

index_record *currentBlock;

int memsize;
int blocks;
int threads_count;
char *filename;
int main(int argc, char **argv)
{
	check_arguments(argc, argv);
	initialize_mutex_and_barrier();
	pthread_t threadId;
	if (pthread_create(&threadId, NULL, map_file_and_launch_threads, NULL) != 0)
	{
		printf("Error creating first thread\n");
		exit(EXIT_FAILURE);
	}
	if (pthread_join(threadId, NULL) != 0)
	{
		printf("Error joining first thread\n");
		exit(EXIT_FAILURE);
	}

	pthread_mutex_destroy(&mutex);
	pthread_barrier_destroy(&barrier);

	read_data_from_file(filename);

	return 0;
}
