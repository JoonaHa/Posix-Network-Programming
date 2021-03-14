#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>

int main(int argc, char **argv)
{

    int listenfd, sockfd, clilen;
    unsigned int port;
    char *end;
    struct sockaddr_in serv_addr, cli_addr;
    int maxBuffer = 1024;
    char buffer[maxBuffer];

    if ((argc != 2) || (port = strtoul(argv[1], &end, 0)) < 0)
    {
        fprintf(stderr, "usage: %s port\n", argv[0]);

        exit(EXIT_FAILURE);
    }

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        fprintf(stderr, "Error creating socket. Errno: %d \n", errno);
        exit(EXIT_FAILURE);
    }
    //Set ip adress and port
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port);

    printf("PORT: %d HTONS %d\n", port, htons(port));

    if (bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        fprintf(stderr, "Error binding local address Errno: %d \n", errno);
        exit(EXIT_FAILURE);
    }
    listen(listenfd, 5);

    //Server loop
    while (1)
    {

        clilen = sizeof(cli_addr);
        sockfd = accept(listenfd, (struct sockaddr *)&cli_addr, &clilen);

        //Connectino errors
        if (sockfd < 0)
        {
            fprintf(stderr, "Error accepting connection errno: %d \n", errno);
            if ((errno == ENETDOWN || errno == EPROTO || errno == ENOPROTOOPT || errno == EHOSTDOWN ||
                 errno == ENONET || errno == EHOSTUNREACH || errno == EOPNOTSUPP || errno == ENETUNREACH))
            {
                continue;
            }
            exit(EXIT_FAILURE);
        }

       /* if ((shutdown(sockfd, SHUT_RDWR)) < 0)
        {
            fprintf(stderr, "Shutdown Errno: %d \n", errno);
            exit(EXIT_FAILURE);
        }

        */
        printf("connection from %s, port %d\n", inet_ntop(AF_INET, &cli_addr.sin_addr, buffer, sizeof(buffer)),
               ntohs(cli_addr.sin_port));

        int bufferLength;
        while (bufferLength = read(sockfd, buffer, maxBuffer) > 0)
        {
            int k = strlen(buffer);
            write(STDOUT_FILENO, buffer, k);
            memset(buffer, 0, maxBuffer);
        }
    }
}
