/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   teht5.c
 * Author: mina
 *
 * Created on 28. tammikuuta 2020, 23:47
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<sys/wait.h>
#include <string.h>

/*
 * 
 */

void reader(int maxBuffer, int *fd) {

    int bufferLength;
    char buffer[maxBuffer];
    //close read
    close(fd[0]);

    while (bufferLength = read(STDOUT_FILENO, buffer, maxBuffer) > 0) {

        if (strlen(buffer) > maxBuffer) {
            close(fd[1]);
            wait(0);
            exit(EXIT_FAILURE);

        } else {

            write(fd[1], buffer, strlen(buffer));
            //write(STDOUT_FILENO, buffer, strlen(buffer));
            memset(buffer, 0, maxBuffer);
        }

    }
    close(fd[1]);
    wait(0);



}

void doubler(int maxBuffer, int *fd) {

    int bufferLength;
    char buffer[maxBuffer];

    //close write end
    close(fd[1]);
    while (bufferLength = read(fd[0], buffer, maxBuffer) > 0) {
        int k = strlen(buffer);
        if (k > maxBuffer) {

            close(fd[0]);
            exit(EXIT_FAILURE);
        } else {


            write(STDOUT_FILENO, buffer, k);
            write(STDOUT_FILENO, buffer, k);
            memset(buffer, 0, maxBuffer);
        }





    }

    close(fd[0]);

}

int main(int argc, char** argv) {
    int maxBuffer = 100;
    int fd[2];

    pid_t childpid;

    if (pipe(fd) == -1) {
        perror("Pipe Failed");
        exit(EXIT_FAILURE);
    }



    childpid = fork();

    //error
    if (childpid < 0) {
        perror("fork Failed");
        exit(EXIT_FAILURE);
        //Parent process
    } else if (childpid > 0) {

        reader(maxBuffer, fd);
        exit(EXIT_SUCCESS);


        // child process 
    } else {

        doubler(maxBuffer, fd);
        exit(EXIT_SUCCESS);
    }


}

