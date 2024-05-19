#include "Record.h"
#include <time.h>

int main()
{
    FILE *file = fopen("records.bin", "wb");
    if (file == NULL)
    {
        perror("fopen");
        return 1;
    }

    char *names[] = {"Ivan", "Petr", "Egor", "Vasili", "Alexey", "Dmitriy", "Andrey", "Nikolai", "Sergei", "Mikhail"};
    char *addresses[] = {"Street 1", "Street 2", "Street 3", "Street 4", "Street 5", "Street 6", "Street 7", "Street 8", "Street 9", "Street 10"};
    int semesters[] = {1, 2, 3, 4, 5, 6, 7, 8};

    srand(time(NULL));

    Record records[COUNT_RECORDS];
    for (int i = 0; i < COUNT_RECORDS; i++)
    {
        snprintf(records[i].name, sizeof(records[i].name), "%s", names[rand() % COUNT_RECORDS]);
        snprintf(records[i].address, sizeof(records[i].address), "%s", addresses[rand() % COUNT_RECORDS]);
        records[i].semester = semesters[rand() % 8];
    }

    if (fwrite(records, sizeof(Record), COUNT_RECORDS, file) != COUNT_RECORDS)
    {
        perror("fwrite");
        return 1;
    }

    fclose(file);
    return 0;
}