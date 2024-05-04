#include "sort_index_utils.h"

pthread_mutex_t mutex;
pthread_barrier_t barrier;
index_hdr_s hdr;
index_record *currentBlock;
int size_to_sort;
int count_blocks;

int main(int argc, char **argv)
{
	Arguments args = check_arguments(argc, argv);
	size_to_sort = args.memsize;
	initialize_mutex_and_barrier(args);
	pthread_t threadId;
	if (pthread_create(&threadId, NULL, map_file_and_launch_threads, (void *)&args) != 0)
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

	return 0;
}
