#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>

int main (void){
    int n, *p;
    int *(pica[10000]) = 1;
    n = shmget (IPC_PRIVATE, sizeof (int), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    p = (int *) shmat (n, 0, 0);
    *p = 1;
    
    pid_t pid_filho1, pid_filho2, pid_filho3;
    
    pid_filho1 = fork();
   
    if (pid_filho1 < 0){
        printf("erro\n");
        exit(1);
    }
    else if (pid_filho1 == 0){
        for(int i = 0; i<1000; i++){
            *p+=1;
            //printf("processo filho1 pid: %d, n= %d\n", getpid(), n);
        }
        printf("processo filho1 pid: %d, n= %d\n", getpid(), *p);
        exit(0);
    }

    pid_filho2=fork();

    if(pid_filho2 < 0){
        printf("erro\n");
        exit(1);
    }
    else if(pid_filho2 == 0){
        for (int i = 0; i<1000; i++){
            *p+=10;
            //printf("processo filho2 pid: %d, n= %d\n", getpid(), n);
        }
        printf("processo filho2 pid: %d, n= %d\n", getpid(), *p);
        exit(0);
    }

    pid_filho3=fork();

    if(pid_filho3 < 0){
        printf("erro\n");
        exit(1);
    }
    else if(pid_filho3 == 0){
        for (int i = 0; i<1000; i++){
            *p+=100;
            //printf("processo filho3 pid: %d, n= %d\n", getpid(), n);
        }
        printf("processo filho3 pid: %d, n= %d\n", getpid(), *p);
        exit(0);
    }
    waitpid(pid_filho1, NULL, 0);
    waitpid(pid_filho2, NULL, 0);
    waitpid(pid_filho3, NULL, 0);
   
    return 0;
}