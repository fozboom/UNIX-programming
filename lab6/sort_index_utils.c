#include "sort_index_utils.h"

void check_arguments(int argc, char *argv[])
{
	if (argc != 5)
	{
		printf("You must start the program:\n ./sort_index <memsize> <blocks> <threads> <filename>  \n");
		exit(EXIT_FAILURE);
	}
	memsize = atoi(argv[1]);
	blocks = atoi(argv[2]);
	threads = atoi(argv[3]);
	filename = argv[4];

	if (memsize <= 0 || memsize % 4096 != 0)
	{
		printf("The memsize must be a multiple of 4096\n");
		exit(EXIT_FAILURE);
	}
	if ((blocks <= 0) || ((blocks & (blocks - 1)) != 0) || (blocks < threads))
	{
		printf("The number of blocks must be a power of 2\n");
		exit(EXIT_FAILURE);
	}
	if (threads < 16 || threads > 8000)
	{
		printf("The number of threads must be between 16 and 8000\n");
		exit(EXIT_FAILURE);
	}
}

void initialize_mutex_and_barrier()
{
	if (pthread_mutex_init(&mutex, NULL) != 0)
	{
		printf("Error creating mutex\n");
		exit(errno);
	}
	if (pthread_barrier_init(&barrier, NULL, threads) != 0)
	{
		printf("Error creating barrier\n");
		exit(errno);
	}
}

void *map_file_and_launch_threads(void *data)
{
	FILE *file = fopen(filename, "rb+");
	if (file == NULL)
	{
		printf("Error opening file\n");
		exit(errno);
	}
	fseek(file, 0, SEEK_END);
	size_t file_size = ftell(file);
	fseek(file, 0, SEEK_SET);

	int file_descriptor = fileno(file);

	void *file_in_memory = mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, file_descriptor, 0);
	if (file_in_memory == MAP_FAILED)
	{
		printf("Error mapping file\n");
		perror(errno);
		exit(errno);
	}

	file_in_memory = (uint64_t *)file_in_memory + 1; // Пропустить кол-во структур
	currentBlock = (index_record *)file_in_memory;
	pthread_t threads[threads - 1];
	for (int i = 1; i < threads; i++)
	{
		thread_data *data = (thread_data *)malloc(sizeof(thread_data));
		data->thread_num = i;
		data->block_size = memsize / blocks;
		data->buffer = (index_record *)file_in_memory;

		if (pthread_create(&threads[i - 1], NULL, sort_block, (void *)data) != 0)
		{
			printf("Error creating thread\n");
			exit(errno);
		}
	}
	thread_data *tmp = (thread_data *)malloc(sizeof(thread_data));
	tmp->block_size = (memsize / blocks);
	tmp->thread_num = 0;
	tmp->buffer = (index_record *)file_in_memory;
	sort_block(tmp);
	for (int i = 1; i < threads; i++)
	{
		if (pthread_join(threads[i - 1], NULL) != 0)
		{
			printf("Error joining thread\n");
			exit(errno);
		}
	}
	munmap(file_in_memory, file_size);

	fclose(file);
	printf("Sorting is done\n");
	return 0;
}
void merge_blocks(index_record *left, index_record *right, index_record *temp, int bufSize)
{
	int leftIndex = 0, rightIndex = 0;
	while (leftIndex < bufSize && rightIndex < bufSize)
	{
		if (left[leftIndex].time_mark < right[rightIndex].time_mark)
		{
			temp[leftIndex + rightIndex] = left[leftIndex];
			leftIndex++;
		}
		else
		{
			temp[leftIndex + rightIndex] = right[rightIndex];
			rightIndex++;
		}
	}
	while (leftIndex < bufSize)
	{
		temp[leftIndex + rightIndex] = left[leftIndex];
		leftIndex++;
	}
	while (rightIndex < bufSize)
	{
		temp[leftIndex + rightIndex] = right[rightIndex];
		rightIndex++;
	}
}

void *sort_block(void *data)
{
	thread_data *args = (thread_data *)data;
	pthread_barrier_wait(&barrier);

	while (currentBlock < args->buffer + memsize)
	{
		pthread_mutex_lock(&mutex);
		if (currentBlock < args->buffer + memsize)
		{
			index_record *block = currentBlock;
			currentBlock += args->block_size;
			pthread_mutex_unlock(&mutex);
			qsort(block, args->block_size, sizeof(index_record), compare);
		}
		else
		{
			pthread_mutex_unlock(&mutex);
			pthread_barrier_wait(&barrier);
			break;
		}
	}

	int count_blocks_to_merge = 2;
	while (count_blocks_to_merge <= blocks)
	{
		pthread_barrier_wait(&barrier);
		currentBlock = args->buffer;
		while (currentBlock < args->buffer + memsize)
		{
			pthread_mutex_lock(&mutex);
			if (currentBlock < args->buffer + memsize)
			{
				index_record *block = currentBlock;
				currentBlock += args->block_size * count_blocks_to_merge;
				pthread_mutex_unlock(&mutex);
				int merge_block_size = args->block_size * (count_blocks_to_merge / 2);
				index_record *left = (index_record *)malloc(merge_block_size * sizeof(index_record));
				index_record *right = (index_record *)malloc(merge_block_size * sizeof(index_record));
				if (left == NULL || right == NULL)
				{
					printf("Error allocating memory\n");
					exit(errno);
				}
				memcpy(left, block, merge_block_size * sizeof(index_record));
				memcpy(right, block + merge_block_size, merge_block_size * sizeof(index_record));

				merge_blocks(left, right, block, merge_block_size);

				free(left);
				free(right);
			}
			else
			{
				pthread_mutex_unlock(&mutex);
				break;
			}
		}
		count_blocks_to_merge *= 2;
	}
	pthread_mutex_unlock(&mutex);
	pthread_barrier_wait(&barrier);

	return 0;
}

int compare(const void *a, const void *b)
{
	index_record *recordA = (index_record *)a;
	index_record *recordB = (index_record *)b;

	if (recordA->time_mark < recordB->time_mark)
		return -1;
	else if (recordA->time_mark > recordB->time_mark)
		return 1;
	else
		return 0;
}

void convert_mjd_to_date(double modified_julian_date, struct tm *date_structure)
{
	// Преобразование модифицированной юлианской даты в количество секунд с начала эпохи Unix (1 января 1970 года)
	time_t seconds_since_epoch = (time_t)((modified_julian_date - 40587.0) * 86400.0);

	// Преобразование секунд с начала эпохи Unix в структуру tm, представляющую собой время в формате UTC
	struct tm *temp_date_structure = gmtime(&seconds_since_epoch);

	// Копирование временной структуры tm в структуру tm, на которую указывает аргумент функции
	*date_structure = *temp_date_structure;
}

void read_data_from_file(const char *filename)
{
	FILE *file = fopen(filename, "rb");
	if (file == NULL)
	{
		printf("Error opening file.\n");
		exit(0);
	}

	index_hdr_s *data = (index_hdr_s *)malloc(sizeof(index_hdr_s));
	if (data == NULL)
	{
		printf("Memory allocation error.\n");
		fclose(file);
		exit(EXIT_FAILURE);
	}

	if (!fread(&data->records, sizeof(uint64_t), 1, file))
	{
		printf("Error while reading records.\n");
		fclose(file);
		exit(EXIT_FAILURE);
	}

	data->idx = (index_record *)malloc(data->records * sizeof(index_record));
	if (data->idx == NULL)
	{
		printf("Memory allocation error.\n");
		fclose(file);
		exit(EXIT_FAILURE);
	}

	if (!fread(data->idx, sizeof(index_record), data->records, file))
	{
		printf("Error while reading idx.\n");
		fclose(file);
		exit(EXIT_FAILURE);
	}

	for (uint64_t i = 0; i < data->records; i++)
	{
		printf("%lf\n", data->idx[i].time_mark);

		struct tm date;
		convert_mjd_to_date(data->idx[i].time_mark, &date);
		printf("%02d-%02d-%d\n\n", date.tm_mday, date.tm_mon + 1, date.tm_year + 1900);
	}

	fclose(file);
}