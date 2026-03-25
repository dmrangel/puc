#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    char input[1024];
    char *args[100];
    int status;

    while (1) {
        printf("$ ");
        if (!fgets(input, 1024, stdin)) break;
        input[strcspn(input, "\n")] = 0;

        int i = 0;
        args[i] = strtok(input, " ");
        while (args[i] != NULL) {
            args[++i] = strtok(NULL, " ");
        }

        if (args[0] == NULL) continue;

        if (fork() != 0) {
            waitpid(-1, &status, 0);
        } else {
            execvp(args[0], args);
            exit(1);
        }
    }
    return 0;
}