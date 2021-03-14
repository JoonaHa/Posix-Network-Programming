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
        write(fd[1], buffer, bufferLength);


    }

    close(fd[1]);

}

void doubler(int maxBuffer, int *fd) {

    int bufferLength;
    char buffer[maxBuffer];

    //close write end
    close(fd[1]);
    while (bufferLength = read(fd[1], buffer, bufferLength) > 0) {
        printf("%s", buffer);
        //write(STDOUT_FILENO, buffer, bufferLength);


    }

    close(fd[0]);

}

int main(int argc, char** argv) {
    int fd[2]; // Used to store two ends of first pipe 

    pid_t childpid;

    if (pipe(fd) == -1) {
        perror("Pipe Failed");
        exit (EXIT_FAILURE);
    }



    childpid = fork();

    if (childpid < 0) {
        perror("fork Failed");
        exit (EXIT_FAILURE);
    }
    else if (childpid > 0) {

        close(fd[0]); // Close reading end of first pipe 
        char input_str[100] = "Hello World";
        // Write input string and close writing end of first 
        // pipe. 
        write(fd[1], input_str, strlen(input_str) + 1);
        close(fd[1]);

        // Wait for child to send a string 
        wait(NULL);


        // Read string from child, print it and close
    } else {

        close(fd[1]); // Close writing end of first pipe 

        // Read a string using first pipe 
        char concat_str[100];
        read(fd[0], concat_str, 100);

        // Concatenate a fixed string with it 
        int k = strlen(concat_str);


        concat_str[k] = '\0'; // string ends with '\0' 

        // Close both reading ends 
        close(fd[0]);

        // Write concatenated string and close writing end 
        printf("Concatenated string %s\n", concat_str);
        exit(EXIT_SUCCESS);
    }




}

