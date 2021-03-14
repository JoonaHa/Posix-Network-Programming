/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: mina
 *
 * Created on 4. helmikuuta 2020, 23:06
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<sys/wait.h>
#include <sys/stat.h> 
#include <sys/types.h> 
#include <string.h>
#include <fcntl.h> 

void reader(int maxBuffer, char* fifoPath) {
    int fd;
    int bufferLength;
    char buffer[maxBuffer];

    //Open fifo
    if ((fd = open(fifoPath, O_WRONLY)) < 0) {
        perror("READER: can't open write fifo");
        exit(EXIT_FAILURE);
    }


    while (1) {

        if (bufferLength = read(STDOUT_FILENO, buffer, maxBuffer) == -1) {
            perror("READER: Couldnt read STDIN ");
            exit(EXIT_FAILURE);
        }

        if (strlen(buffer) <= maxBuffer) {

            write(fd, buffer, maxBuffer);
            memset(buffer, 0, maxBuffer);
        } else {
            continue;
        }


    }


    close(fd);

}

/*
 * 
 */
int main(int argc, char** argv) {
    int maxBuffer = 100;

    //Create fifo

    char myfifo[] = "./myfifo1";

    if (mkfifo(myfifo, 0666) < 0) {
        //perror("READER: Couldn't create fifo1.");
    }

    reader(maxBuffer, myfifo);




    if (unlink(myfifo) < 0)
        perror("READER: can't unlink");
    exit(EXIT_SUCCESS);

}

