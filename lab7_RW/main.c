#include "RecordUtilc.h"

int file_descriptor = 0;
bool record_modified = false;
Record record;
Record record_copy;
Record record_new;

int main()
{
    bool continue_flag = true;
    if (open_file("records.bin") != 0)
    {
        exit(EXIT_FAILURE);
    }
    print_menu();
    processUserInput();
}