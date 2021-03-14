/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   teht2.c
 * Author: mina
 *
 * Created on 20. tammikuuta 2020, 21:13
 */

#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/*
 * 
 */
int main(int argc, char** argv) {



    int bufferLength;
    char buffer[500];
    memset(buffer, 0, 500);
    while (bufferLength = read(0, buffer, 500) > 0) {
        //char message[1000];
        //strcat(message, buffer);
        write(STDOUT_FILENO, buffer, strlen(buffer));
        write(STDOUT_FILENO, buffer, strlen(buffer));
        memset(buffer, 0, 500);


    }



    return (EXIT_SUCCESS);
}