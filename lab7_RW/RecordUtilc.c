#include "RecordUtilc.h"

void print_record(Record _record, int record_number)
{
    printf("-------------------------------------------------\n");
    printf("Record number: %d\n", record_number);
    printf("-------------------------------------------------\n");
    printf("Name: %s\n", _record.name);
    printf("Address: %s\n", _record.address);
    printf("Semester: %d\n", _record.semester);
    printf("-------------------------------------------------\n\n");
}

UserInput parseUserInput(char *input)
{
    UserInput userInput;
    char *token = strtok(input, " ");
    strncpy(userInput.command, token, sizeof(userInput.command));
    token = strtok(NULL, " ");
    userInput.argument = token ? strtol(token, NULL, 10) : -1;
    return userInput;
}

int open_file(const char *filename)
{
    file_descriptor = open(filename, O_RDWR);
    if (file_descriptor == -1)
    {
        perror("Error: file open failed\n");
        return 1;
    }
    return 0;
}

int lock_file(int file_descriptor, int record_number, struct flock *lock_params)
{
    lock_params->l_type = F_WRLCK;
    lock_params->l_whence = SEEK_SET;
    lock_params->l_start = (record_number - 1) * sizeof(Record);
    lock_params->l_len = sizeof(Record);
    if (fcntl(file_descriptor, F_SETLKW, lock_params) == -1)
    {
        perror("Error: file lock failed\n");
        return -1;
    }
    return 0;
}

int unlock_file(int file_descriptor, struct flock *lock_params)
{
    lock_params->l_type = F_UNLCK;
    if (fcntl(file_descriptor, F_SETLKW, lock_params) == -1)
    {
        perror("Error: file unlock failed\n");
        return -1;
    }
    return 0;
}

int lock_file_whole(int file_descriptor, struct flock *lock_params)
{
    lock_params->l_type = F_RDLCK;
    lock_params->l_whence = SEEK_SET;
    lock_params->l_start = 0;
    lock_params->l_len = 0;
    if (fcntl(file_descriptor, F_SETLKW, lock_params) == -1)
    {
        perror("Error: file lock failed\n");
        return -1;
    }
    return 0;
}

int print_file_content()
{
    struct flock lock_params;
    if (lock_file_whole(file_descriptor, &lock_params) == -1)
    {
        exit(EXIT_FAILURE);
    }
    int record_number = 1;
    printf(GREEN_COLOR);
    Record record;
    while (read(file_descriptor, &record, sizeof(Record)) > 0)
    {
        print_record(record, record_number);
        record_number++;
    }
    printf(RESET_COLOR);
    sleep(10);
    if (unlock_file(file_descriptor, &lock_params) == -1)
    {
        exit(EXIT_FAILURE);
    }
    lseek(file_descriptor, 0, SEEK_SET);
    return 0;
}

bool is_record_number_valid(int record_number)
{
    return record_number > 0 && record_number <= COUNT_RECORDS;
}

void get_record(int record_number, Record *record)
{
    if (!is_record_number_valid(record_number))
    {
        printf(RED_COLOR);
        printf("Error: record number must be greater than 0\n");
        printf(RESET_COLOR);
        return;
    }
    struct flock lock_params;
    if (lock_file(file_descriptor, record_number, &lock_params) == -1)
    {
        exit(EXIT_FAILURE);
    }
    lseek(file_descriptor, (record_number - 1) * sizeof(Record), SEEK_SET);
    read(file_descriptor, record, sizeof(Record));
    strcpy(record_copy.name, record->name);
    strcpy(record_copy.address, record->address);
    record_copy.semester = record->semester;
    if (unlock_file(file_descriptor, &lock_params) == -1)
    {
        exit(EXIT_FAILURE);
    }
    lseek(file_descriptor, 0, SEEK_SET);
}

bool records_are_equal(Record record1, Record record2)
{
    return strcmp(record1.name, record2.name) == 0 && strcmp(record1.address, record2.address) == 0 && record1.semester == record2.semester;
}

void modify_record(int record_number)
{
    if (!is_record_number_valid(record_number))
    {
        printf(RED_COLOR);
        printf("Error: record number must be greater than 0\n");
        printf(RESET_COLOR);
        return;
    }

    struct flock lock_params;
    if (lock_file(file_descriptor, record_number, &lock_params) == -1)
    {
        exit(EXIT_FAILURE);
    }

    lseek(file_descriptor, (record_number - 1) * sizeof(Record), SEEK_SET);
    read(file_descriptor, &record, sizeof(Record));

    strcpy(record_copy.name, record.name);
    strcpy(record_copy.address, record.address);
    record_copy.semester = record.semester;

    printf("Enter the name: ");
    fgets(record.name, sizeof(record.name), stdin);
    record.name[strcspn(record.name, "\n")] = 0;

    printf("Enter the address: ");
    fgets(record.address, sizeof(record.address), stdin);
    record.address[strcspn(record.address, "\n")] = 0;

    printf("Enter the semester: ");
    scanf("%d", &record.semester);
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }

    if (!records_are_equal(record, record_copy))
    {
        printf("Record has been modified\n");
        record_modified = true;
    }
    else
    {
        printf(RED_COLOR);
        printf("Error: record has not been modified\n");
        printf(RESET_COLOR);
    }
    sleep(5);
    if (unlock_file(file_descriptor, &lock_params) == -1)
    {
        exit(EXIT_FAILURE);
    }
    lseek(file_descriptor, 0, SEEK_SET);
}

void put_record(int record_number)
{
    if (!is_record_number_valid(record_number))
    {
        printf(RED_COLOR);
        printf("Error: record number must be greater than 0\n");
        printf(RESET_COLOR);
        return;
    }

    if (record_modified)
    {
        struct flock lock_params;
        if (lock_file(file_descriptor, record_number, &lock_params) == -1)
        {
            exit(EXIT_FAILURE);
        }

        get_record(record_number, &record_new);
        if (!records_are_equal(record_copy, record_new))
        {
            if (unlock_file(file_descriptor, &lock_params) == -1)
            {
                exit(EXIT_FAILURE);
            }
            printf(RED_COLOR);
            printf("Record has been modified by another user\n");
            printf(RESET_COLOR);
            return;
        }
        lseek(file_descriptor, (record_number - 1) * sizeof(Record), SEEK_SET);
        write(file_descriptor, &record, sizeof(Record));
        record_modified = false;

        if (unlock_file(file_descriptor, &lock_params) == -1)
        {
            exit(EXIT_FAILURE);
        }
        lseek(file_descriptor, 0, SEEK_SET);
        record_modified = false;
    }
    else
    {
        printf(RED_COLOR);
        printf("Error: record has not been modified\n");
        printf(RESET_COLOR);
    }
}

void print_menu()
{
    printf(BLUE_COLOR);
    printf("_________________________________________________________________\n");
    printf("|                                                               |\n");
    printf("| LST       - Display the contents of the file                  |\n");
    printf("| GET <num> - Get the record with the ordinal number            |\n");
    printf("| MOD <num> - Modify the fields of the record                   |\n");
    printf("| PUT       - Save the last read or modified record in place    |\n");
    printf("|                                                               |\n");
    printf("|_______________________________________________________________|\n");
    printf(RESET_COLOR);
}

void processUserInput()
{
    char input[100];
    int last_record_number = -1;
    do
    {
        printf("Your choice: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;
        UserInput userInput = parseUserInput(input);
        if (strcmp(userInput.command, "LST") == 0)
        {
            print_file_content();
        }
        else if (strcmp(userInput.command, "GET") == 0)
        {
            if (userInput.argument == -1)
            {
                printf(RED_COLOR);
                printf("Error: put GET <num>\n");
                printf(RESET_COLOR);
                return;
            }
            get_record(userInput.argument, &record);
            print_record(record, userInput.argument);
            last_record_number = userInput.argument;
        }
        else if (strcmp(userInput.command, "MOD") == 0)
        {
            if (userInput.argument == -1)
            {
                printf(RED_COLOR);
                printf("Error: put MOD <num>\n");
                printf(RESET_COLOR);
                return;
            }
            modify_record(userInput.argument);
            printf("Record has been modified fdsfds\n");
            last_record_number = userInput.argument;
        }
        else if (strcmp(userInput.command, "PUT") == 0)
        {
            put_record(last_record_number);
        }
        else if (strcmp(userInput.command, "EXIT") == 0)
        {
            exit(EXIT_SUCCESS);
        }
        else
        {
            printf(RED_COLOR);
            printf("Unknown command\n");
            printf(RESET_COLOR);
        }
    } while (true);
}