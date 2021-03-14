/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   teht3.c
 * Author: mina
 *
 * Created on 21. tammikuuta 2020, 14:39
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


/*
 * 
 */
int main(int argc, char** argv) {

    if (argc == 2) {

        struct stat fileInfo;

        if (stat(argv[1], &fileInfo) == -1)
            return (EXIT_FAILURE);

        printf("File type: %o\n", fileInfo.st_mode);
        printf("Owner id: %d\n", fileInfo.st_uid);
        printf("Block size: %ld\n", fileInfo.st_blksize);
    } else {
        printf("Usage: %s <pathname>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    return (EXIT_SUCCESS);
}

