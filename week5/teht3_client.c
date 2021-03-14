
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

void handle_sigint(int sig)
{
    char message[] = "Caugh signal:";
    write(STDOUT_FILENO, message, sizeof(message));
    exit(EXIT_FAILURE);
}

// TODO add SIGPIPE handler
int main(int argc, char **argv)
{
    int sockfd, n;

    struct sockaddr_in servaddr;
    char *end;
    unsigned int port;
    int maxBuffer = 1024;
    char buffer[maxBuffer];

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
    inet_pton(AF_INET, argv[2], &servaddr.sin_addr);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("socket error");
        exit(EXIT_FAILURE);
    }
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        printf("connect error");
        exit(EXIT_FAILURE);
    }

    /*if ((shutdown(sockfd, SHUT_RD)) < 0)
    {
        fprintf(stderr, "Shutdown Errno: %d \n", errno);
        exit(EXIT_FAILURE);
    }
    */
    char buf[100];
    int bytes;
    while (read(STDOUT_FILENO, buffer, maxBuffer) > 0)
    {

        if ((bytes = write(sockfd, buffer, maxBuffer)) < 0)
        {
            fprintf(stderr, "write errno: %d\n", errno);
            exit(EXIT_FAILURE);
        }
        sprintf(buf, "%d", bytes);
        buf[strlen(buf) + 1] = '\n';
        write(STDOUT_FILENO, buf, strlen(buffer));
        memset(buffer, 0, maxBuffer);
    }
}
