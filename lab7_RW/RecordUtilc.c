#include "RecordUtilc.h"

UserInput parseUserInput(char *input) {
    UserInput userInput;
    char *token = strtok(input, " ");
    strncpy(userInput.command, token, sizeof(userInput.command));
    token = strtok(NULL, " ");
    userInput.argument = token ? strtol(token, NULL, 10) : -1;
    return userInput;
}

int open_file(const char *filename) {
    file_descriptor = open(filename, O_RDWR);
    if (file_descriptor == -1) {
        perror("Error: file open failed\n");
        return 1;
    }
    return 0;
}

int print_file_content() {
    struct flock lock_params;
    lock_params.l_type = F_RDLCK;
    lock_params.l_whence = SEEK_SET;
    lock_params.l_start = 0;
    lock_params.l_len = 0;
    if (fcntl(file_descriptor, F_SETLK, &lock_params) == -1) {
        perror("Error: file lock failed\n");
        return 1;
    }
    int record_number = 1;
    printf(GREEN_COLOR);
    Record record;
    while (read(file_descriptor, &record, sizeof(Record)) > 0) {
        print_record(record, record_number);
        record_number++;
    }
    printf(RESET_COLOR);
    sleep(3);
    lock_params.l_type = F_UNLCK;
    if (fcntl(file_descriptor, F_SETLK, &lock_params) == -1) {
        perror("Error: file unlock failed\n");
        return 1;
    }
    lseek(file_descriptor, 0, SEEK_SET);
    return 0;
}

void print_record(Record _record, int record_number) {
    printf("-------------------------------------------------\n");
    printf("Record number: %d\n", record_number);
    printf("-------------------------------------------------\n");
    printf("Name: %s\n", _record.name);
    printf("Address: %s\n", _record.address);
    printf("Semester: %d\n", _record.semester);
    printf("-------------------------------------------------\n\n");
}

void get_record(int record_number, Record* record) {
    if (record_number <= 0 || record_number > COUNT_RECORDS) {
        printf(RED_COLOR);
        printf("Error: record number must be greater than 0\n");
        printf(RESET_COLOR);
        return;
    }
    struct flock lock_params;
    lock_params.l_type = F_RDLCK;
    lock_params.l_whence = SEEK_SET;
    lock_params.l_start = (record_number - 1) * sizeof(Record);
    lock_params.l_len = sizeof(Record);
    if (fcntl(file_descriptor, F_SETLK, &lock_params) == -1) {
        perror("Error: file lock failed\n");
        return;
    }
    lseek(file_descriptor, (record_number - 1) * sizeof(Record), SEEK_SET);
    read(file_descriptor, record, sizeof(Record));
    print_record(*record, record_number);
    lock_params.l_type = F_UNLCK;
    if (fcntl(file_descriptor, F_SETLK, &lock_params) == -1) {
        perror("Error: file unlock failed\n");
        return;
    }
    lseek(file_descriptor, 0, SEEK_SET);
}

bool records_are_equal(Record record1, Record record2) {
    return strcmp(record1.name, record2.name) == 0 && strcmp(record1.address, record2.address) == 0 && record1.semester == record2.semester;
}

void modify_record(int record_number) {
    if (record_number <= 0 || record_number > COUNT_RECORDS) {
        printf(RED_COLOR);
        printf("Error: record number must be greater than 0\n");
        printf(RESET_COLOR);
        return;
    }

    struct flock lock_params;
    lock_params.l_type = F_WRLCK;
    lock_params.l_whence = SEEK_SET;
    lock_params.l_start = (record_number - 1) * sizeof(Record);
    lock_params.l_len = sizeof(Record);
    if (fcntl(file_descriptor, F_SETLK, &lock_params) == -1) {
        perror("Error: file lock failed\n");
        return;
    }

    lseek(file_descriptor, (record_number - 1) * sizeof(Record), SEEK_SET);
    read(file_descriptor, &record, sizeof(Record));

    record_copy = record;

    printf("Enter the name: ");
    fgets(record.name, sizeof(record.name), stdin);
    record.name[strcspn(record.name, "\n")] = 0;

    printf("Enter the address: ");
    fgets(record.address, sizeof(record.address), stdin);
    record.address[strcspn(record.address, "\n")] = 0;

    printf("Enter the semester: ");
    scanf("%hhu", &record.semester);

    if(!records_are_equal(record, record_copy)) {
        record_modified = true;
    }

    lseek(file_descriptor, (record_number - 1) * sizeof(Record), SEEK_SET);
    write(file_descriptor, &record, sizeof(Record));
    lock_params.l_type = F_UNLCK;
    if (fcntl(file_descriptor, F_SETLK, &lock_params) == -1) {
        perror("Error: file unlock failed\n");
        return;
    }
    lseek(file_descriptor, 0, SEEK_SET);
}

void put_record(int record_number) {
    if (record_number <= 0 || record_number > COUNT_RECORDS) {
        printf(RED_COLOR);
        printf("Error: record number must be greater than 0\n");
        printf(RESET_COLOR);
        return;
    }

    if (record_modified) {
        struct flock lock_params;
        lock_params.l_type = F_WRLCK;
        lock_params.l_whence = SEEK_SET;
        lock_params.l_start = (record_number - 1) * sizeof(Record);
        lock_params.l_len = sizeof(Record);
        if (fcntl(file_descriptor, F_SETLK, &lock_params) == -1) {
            perror("Error: file lock failed\n");
            return;
        }

        get_record(record_number, &record_new);
        if (!records_are_equal(record_copy, record_new))
        {
            lock_params.l_type = F_UNLCK;
            if (fcntl(file_descriptor, F_SETLK, &lock_params) == -1) {
                perror("Error: file unlock failed\n");
                return;
            }
            printf(RED_COLOR);
            printf("Record has been modified by another user\n");
            printf(RESET_COLOR);
            return;
        }
        lseek(file_descriptor, (record_number - 1) * sizeof(Record), SEEK_SET);
        write(file_descriptor, &record, sizeof(Record));
        record_modified = false;

        lock_params.l_type = F_UNLCK;
        if (fcntl(file_descriptor, F_SETLK, &lock_params) == -1) {
            perror("Error: file unlock failed\n");
            return;
        }
        lseek(file_descriptor, 0, SEEK_SET);

    }
    else {
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
    printf("| MOD       - Modify the fields of the record                   |\n");
    printf("| PUT       - Save the last read and modified record in place   |\n");
    printf("|                                                               |\n");
    printf("|_______________________________________________________________|\n");
    printf(RESET_COLOR);    
}

void processUserInput() {
    char input[100];
    printf("Your choice: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    printf("%s\n", input);
    UserInput userInput = parseUserInput(input);
    printf("Command: %s\n", userInput.command);
    if (strcmp(userInput.command, "LST") == 0) {
        print_file_content();
    }
    else if (strcmp(userInput.command, "GET") == 0) {
        printf("GET command\n");
    }
    else if (strcmp(userInput.command, "MOD") == 0) {
        printf("MOD command\n");
    }
    else if (strcmp(userInput.command, "PUT") == 0) {
        printf("PUT command\n");
    }
    else if (strcmp(userInput.command, "EXIT") == 0) {
        exit(EXIT_SUCCESS);
    }
    else {
        printf(RED_COLOR);
        printf("Unknown command\n");
        printf(RESET_COLOR);
    }
}