#include <stdio.h>
#include <stdlib.h>

extern char **environ;

int main() {
    char **env_var = environ;
    while (*env_var != NULL) {
        printf("%s\n", *env_var);
        env_var++;
    }

    printf("%s", getenv("CHILD_PATH"));
    return 0;
}
