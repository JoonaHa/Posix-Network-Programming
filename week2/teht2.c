/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: mina
 *
 * Created on 28. tammikuuta 2020, 20:16
 */

#include <stdio.h>
#include <stdlib.h>
#include<sys/wait.h>
#include <unistd.h>

/*
 * 
 */

int do_child(char *path, char *arg) {

    int err;
    char *arr[] = {path, arg};
    err = execv(path, arr);


    if (err < 0)
        return (err);

}

int main(int argc, char** argv) {


    if (argc < 2) {

        printf("Usage: %s <pathname> <optional arg>\n", argv[0]);
        exit(EXIT_FAILURE);
    }


    int pid;
    int i;

    //Fork failure
    if ((pid = fork()) < 0) {
        exit(EXIT_FAILURE);
    }
    //Parent program
    if (pid != 0) {

        for (i = 1; i <= 5; i++)
            printf(" i = %d \n", i);

        pid_t childPid;
        while (childPid = wait(0) != pid);
        printf("Child process exit status: %d\n", childPid);
        exit(EXIT_SUCCESS);
    }// child process
    else {

        if (argc == 2)
            do_child(argv[1], NULL);
        if (argc == 3)
            do_child(argv[1], argv[2]);
    }


}

