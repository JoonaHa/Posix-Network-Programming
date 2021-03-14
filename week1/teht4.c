/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   teht4.c
 * Author: mina
 *
 * Created on 21. tammikuuta 2020, 15:11
 */

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include<signal.h>

/*
 * 
 */

void handle_sigint(int sig) {
    char message[] = "\n\nCaught signal\n";
    write(STDOUT_FILENO, message, sizeof (message));
    exit(EXIT_FAILURE);
}

int main(int argc, char** argv) {

    struct sigaction act;
    act.sa_handler = handle_sigint;
    sigaction(SIGINT, &act, NULL);

    //signal(SIGINT, handle_sigint);



    if (argc == 2) {

        int bufferLength;
        char buffer[1024];

        int fileD = open(argv[1], O_RDONLY);



        if (fileD == -1)
            exit(EXIT_FAILURE);

        while (bufferLength = read(fileD, buffer, sizeof (*buffer)) > 0) {

            write(STDOUT_FILENO, buffer, bufferLength);
            usleep(300);
        }
        close(fileD);

    } else {
        exit(EXIT_FAILURE);
    }

    return (EXIT_SUCCESS);
}

