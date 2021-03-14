#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
void handle_sigint(int sig)
{
    char message[32];
    char str[64];
    strcpy(message, "Caugh signal:");
    sprintf(str, "%d", sig);
    strcat(message, str);
    write(STDOUT_FILENO, message, strlen(message));
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    int sockfd, n;

    struct sockaddr_in servaddr;
    char *end;
    unsigned int port;
    int MAXLINE;

    struct sigaction act;
    act.sa_handler = handle_sigint;
    sigaction(SIGPIPE, &act, NULL);
    if ((argc != 3) || (port = strtoul(argv[2], &end, 0)) < 0)
    {
        fprintf(stderr, "usage: %s ip port\n", argv[0]);

        exit(EXIT_FAILURE);
    }
    printf("PORT: %d HTONS %d\n", port, htons(port));

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("socket error");
        exit(EXIT_FAILURE);
    }

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        fprintf(stderr, "Connect errno:%d\n", errno);
        exit(EXIT_FAILURE);
    }

    char buff[64];
    while (read(STDOUT_FILENO, buff, 64) > 0)
    {
        MAXLINE = atoi(buff);
        char sendline[MAXLINE];
        memset(&sendline, 0, MAXLINE);

        if ((sendto(sockfd, sendline, MAXLINE, 0, (struct sockaddr *)&servaddr, sizeof(servaddr))) < 0)
            fprintf(stderr, "Sendto errno:%d\n", errno);
    }
}