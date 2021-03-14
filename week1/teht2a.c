/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   teht2a_1.c
 * Author: mina
 *
 * Created on 22. tammikuuta 2020, 0:13
 */

#include <stdio.h>
#include <stdlib.h>

/*
 * 
 */
int main(int argc, char** argv) {
    
    
    if (argc == 2) {

        FILE * filep;
        char *line = NULL;
        size_t len = 0;


        filep = fopen(argv[1], "r");

        if (filep == NULL)
            exit(EXIT_FAILURE);


        while (getline(&line, &len, filep) != -1) {

            printf("%s", line);
            printf("%s\n", line);
        }



    } else {
        while (1) {
            char readStr[255];

            scanf("%s", readStr);

            printf("%s", readStr);
            printf("%s\n", readStr);

        }
    }

    return (EXIT_SUCCESS);
}

