#ifndef INDEX_DATA_H
#define INDEX_DATA_H

#include <stdio.h>
#include <stdint.h>

typedef struct {
    double time_mark;		// временная метка (модифицированная юлианская дата)
    uint64_t recno;			// первичный индекс в таблице БД
} index_record;

typedef struct {
    uint64_t records;		// количество записей
    index_record * idx;		// массив записей в количестве records
} index_hdr_s;

#endif