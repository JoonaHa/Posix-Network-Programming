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

void converter(int maxBuffer, char* fifoPath1, char* fifoPath2) {
    int readFd;
    int writeFd;
    char buffer[maxBuffer];

    //Open fifos
    if ((readFd = open(fifoPath1, O_RDONLY)) < 0) {
        perror("CONVERTER: can't open read fifo");
        exit(EXIT_FAILURE);
    }

    if ((writeFd = open(fifoPath2, O_WRONLY)) < 0) {
        perror("CONVERTER: can't open write fifo");
        exit(EXIT_FAILURE);
    }

    //Read and convert characters
    while (1) {

        read(readFd, buffer, maxBuffer);

        for (int i = 0; buffer[i] != 0; i++) {
            if (isupper(buffer[i]))
                buffer[i] = tolower(buffer[i]);
            else
                buffer[i] = toupper(buffer[i]);
        }

        write(writeFd, buffer, strlen(buffer));
        memset(buffer, 0, maxBuffer);

    }

    close(readFd);
    close(writeFd);
    if (unlink(fifoPath2) < 0)
        perror("CONVERTER: can't unlink");
}

/*
 * 
 */
int main(int argc, char** argv) {
    int maxBuffer = 100;

    //Create fifo

    char myfifo1[] = "./myfifo1";
    char myfifo2[] = "./myfifo2";

    if (mkfifo(myfifo1, 0666) < 0) {
        perror("CONVERTER: Couldn't create fifo1.");
    }
    if (mkfifo(myfifo2, 0666) < 0) {
        perror("CONVERTER: Couldn't create fifo2.");
    }

    converter(maxBuffer, myfifo1, myfifo2);




    exit(EXIT_SUCCESS);

}

