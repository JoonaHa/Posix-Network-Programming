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
#include<ctype.h>

void doubler(int maxBuffer, char* fifoPath1) {
    int readFd;
    char buffer[maxBuffer];

    //Open fifo
    if ((readFd = open(fifoPath1, O_RDONLY)) < 0) {
        perror("DOUBLER: can't open read fifo");
        exit(EXIT_FAILURE);
    }


    //Write doubler
    while (1) {

        read(readFd, buffer, maxBuffer);
        int length = strlen(buffer);
        printf("%d\n", length);

        write(STDOUT_FILENO, buffer, length);
        write(STDOUT_FILENO, buffer, length);
        memset(buffer, 0, maxBuffer);

    }

    close(readFd);

}

/*
 * 
 */
int main(int argc, char** argv) {
    int maxBuffer = 100;

    //Create fifo

    char myfifo1[] = "./myfifo2";

    if (mkfifo(myfifo1, 0666) < 0)
        perror("DOUBLER: Couldn't create write fifo2.");


    doubler(maxBuffer, myfifo1);




    exit(EXIT_SUCCESS);

}

