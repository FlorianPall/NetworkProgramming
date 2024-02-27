//
// Created by Florian Pallmann on 26.02.24.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int main(int argc, char *argv[]) {
    int i;
    pid_t pid;
    int status;
    int maxprocs = 5;
    int procs;
    // Initialisiert C random number generator
    srand((unsigned int) time(NULL));
    // Spawnt 5 Prozesse
    for(procs=0; procs < maxprocs; procs++) {
        // Generiert eine Zahl von 1 bis 10 für jeden Fork
        int rnd = rand()%10+1;
        // Spawnt die Prozesse
        if ((pid = fork()) < 0) {
            printf("error forking\n");
            return 0;
        }
        if (pid == 0) {
            // Child
            // Code des Prozesses und anzahl an sekunden
            printf("Process %d started with sleeptime %d\n", procs, rnd);
            // Schleife mit zufälliger Zeit am Schlafen
            for(i=0;i<5;i++) {
                printf("Child %d in loop number %d\n", procs, i);
                sleep(rnd);
            }
            printf("End of child procs %d\n", procs);
            return(0);
        }
    }
    // Wartet bis alle Prozesse ausgeführt wurden
    while(waitpid(-1,&status,0) > 0) ;
    // Zeigt an ob der Parent fertig ist
    printf("Parent completed\n");
    return 0; }