/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: mina
 *
 * Created on 18. tammikuuta 2020, 14:55
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <unistd.h>
#include <sys/time.h>

/*
 * 
 */
int main(int argc, char *argv[], char *envp[]) {

    struct timeval t0;
    gettimeofday(&t0, 0);
    pid_t PID = getpid();
    pid_t parentPID = getppid();
    struct timeval t1;


    printf("The process id is %d\n", PID);
    printf("The parent process id is %d\n", parentPID);
    
    gettimeofday(&t1, 0);
    printf("Time: %ldms \n\n", t1.tv_usec - t0.tv_usec);

    printf("Count of enviroment  variables: %ld\n", (sizeof (*envp) / sizeof (**envp)));
    printf("Enviroment variables:\n");
    
    gettimeofday(&t0, 0);

    while (*envp) {
        printf("%s\n", *envp++);
    }
    
    gettimeofday(&t1, 0);
    printf("Time: %ldms \n\n", t1.tv_usec - t0.tv_usec);


    return (EXIT_SUCCESS);
}

