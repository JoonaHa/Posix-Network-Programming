#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv)
{
    int sockfd, port;
    struct sockaddr_in servaddr, cliaddr;
    int MAXLINE;
    if (argc != 3)
    {
        fprintf(stderr, "usage: %s port datagram\n", argv[0]);

        exit(EXIT_FAILURE);
    }
    port = atoi(argv[1]);
    MAXLINE = atoi(argv[2]);

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, PF_UNSPEC)) < 0)
        printf("server: can’t create datagram socket");

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        printf("server: can’t bind datagram socket");

    int n;
    int len;
    char mesg[MAXLINE];
    
    for (;;)
    {
        len = sizeof(cliaddr);
        n = recvfrom(sockfd, mesg, MAXLINE, 0, (struct sockaddr *)&cliaddr, &len);
        if (n < 0)
            printf("recvfrom error");

        printf("Received %d bytes of data\n", n);
    }
}
