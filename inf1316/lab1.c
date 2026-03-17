#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
    int n = 1;
    pid_t pid_filho = fork();
 
    if (pid_filho == 0) {
        pid_t pid_neto = fork();
 
        if (pid_neto == 0) {
            for (int i = 0; i < 1000; i++) n += 100;
            printf("processo neto,  pid=%d, n=%d\n", getpid(), n);
            exit(0);
        }
 
        for (int i = 0; i < 1000; i++) n += 10;
        printf("processo filho, pid=%d, n=%d\n", getpid(), n);
        wait(NULL);
        exit(0);
    }
 
    for (int i = 0; i < 1000; i++) n += 1;
    printf("processo pai,   pid=%d, n=%d\n", getpid(), n);
    wait(NULL);
 
    return 0;
}