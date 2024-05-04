#include "index_data.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("You should start the program: ./read_index <filename>\n");
		exit(0);
	}

	FILE *f = fopen(argv[1], "rb");
	if (f == NULL)
	{
		printf("Error opening file.\n");
		exit(0);
	}

	index_hdr_s *data = (index_hdr_s *)malloc(sizeof(index_hdr_s));
	if (data == NULL)
	{
		printf("Memory allocation error.\n");
		fclose(f);
		exit(0);
	}

	if (!fread(&data->records, sizeof(uint64_t), 1, f))
	{
		printf("Error while reading records.\n");
		fclose(f);
		exit(0);
	}

	data->idx = (index_record *)malloc(data->records * sizeof(index_record));
	if (data->idx == NULL)
	{
		printf("Memory allocation error.\n");
		fclose(f);
		exit(0);
	}

	if (!fread(data->idx, sizeof(index_record), data->records, f))
	{
		printf("Error while reading idx.\n");
		fclose(f);
		exit(0);
	}

	for (uint64_t i = 0; i < data->records; i++)
	{
		printf("%lf\n", data->idx[i].time_mark);

		struct tm date;
		convert_mjd_to_date(data->idx[i].time_mark, &date);
		printf("%d-%02d-%02d\n\n", date.tm_year + 1900, date.tm_mon + 1, date.tm_mday);
	}

	fclose(f);
	return 0;
}

void convert_mjd_to_date(double modified_julian_date, struct tm *date_structure)
{
	time_t seconds_since_epoch = (time_t)((modified_julian_date - 40587.0) * 86400.0);
	gmtime_r(&seconds_since_epoch, date_structure);
}
