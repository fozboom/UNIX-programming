#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "index_data.h"

#define START_DATE 15020
#define END_DATE 60433 // 60433 - 3 мая 2023 года
#define SECONDS_IN_HOUR 3600
#define MINUTES_IN_HOUR 60
#define HOURS_IN_HALF_DAY 12

double generate_time_mark();
void generate_index_file(uint64_t records, char *filename);

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		printf("You must start the program:\n ./index_generator <number of entries> <output file>\n");
		exit(EXIT_FAILURE);
	}
	srand(time(NULL));

	uint64_t records = atoll(argv[1]);
	if (records % 256 != 0)
	{
		printf("The file size must be a multiple of 16 bytes\n");
		exit(EXIT_FAILURE);
	}
	generate_index_file(records, argv[2]);

	return 0;
}

void generate_index_file(uint64_t records, char *filename)
{
	srand(time(NULL));

	index_hdr_s hdr;
	hdr.records = records;
	if (hdr.records % 256 != 0)
	{
		printf("The number of records must be a multiple of 256\n");
		exit(EXIT_FAILURE);
	}
	hdr.idx = (index_record *)malloc(hdr.records * sizeof(index_record));
	if (hdr.idx == NULL)
	{
		printf("Memory allocation error\n");
		exit(EXIT_FAILURE);
	}
	for (uint64_t i = 0; i < hdr.records; i++)
	{
		hdr.idx[i].recno = i + 1; // первичный индекс заполняется последовательно с 1
		hdr.idx[i].time_mark = generate_time_mark();
	}
	FILE *fileDescriptor = fopen(filename, "wb");
	if (fileDescriptor == NULL)
	{
		printf("Error opening file\n");
		exit(EXIT_FAILURE);
	}
	fwrite(&hdr.records, sizeof(hdr.records), 1, fileDescriptor);
	fwrite(hdr.idx, sizeof(index_record), hdr.records, fileDescriptor);

	fclose(fileDescriptor);
	free(hdr.idx);
}

double generate_time_mark()
{
	double time_mark = 0.0;
	// генерация временной метки в диапазоне от 1 января 1900 года до вчерашнего дня
	time_mark = START_DATE + rand() % (END_DATE - START_DATE + 1);
	// добавляем случайное время в пределах 12 часов
	time_mark += 0.5 * ((rand() % HOURS_IN_HALF_DAY) * SECONDS_IN_HOUR + (rand() % MINUTES_IN_HOUR) * MINUTES_IN_HOUR + rand() % MINUTES_IN_HOUR) / (HOURS_IN_HALF_DAY * SECONDS_IN_HOUR);
	// Деление общего количество секунд на количество секунд в половине дня, чтобы получить долю дня.
	return time_mark;
}