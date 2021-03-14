#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>

int main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_in sa;
    struct servent *service_name;
    struct hostent *host_name;

    if (argc != 3)
    {
        fprintf(stderr, "usage: %s <hostname> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    //Get address from hostname
    if ((host_name = gethostbyname(argv[1])) == NULL)
    {
        fprintf(stderr, "%s: cannot find host %s\n", argv[0], argv[1]);
        exit(EXIT_FAILURE);
    }

    //Copy port and ip-address to sockaddr_in
    memset(&sa, 0, sizeof(sa));
    sa.sin_port = htons(atoi(argv[2]));
    sa.sin_family = host_name->h_addrtype;
    bcopy(host_name->h_addr, &sa.sin_addr.s_addr, host_name->h_length);
    char *ip = inet_ntoa(sa.sin_addr);
    printf("Host adress: %s Ip adress: %s\n", (char *)argv[1], ip);
    printf("Port: %s HTONS: %d\n", (char *)argv[2], sa.sin_port);

    if ((sockfd = socket(host_name->h_addrtype, SOCK_STREAM, 0)) < 0)
    {
        perror("socket\n");
        exit(EXIT_FAILURE);
    }

    if (connect(sockfd, (struct sockaddr *)&sa, sizeof sa) < 0)
    {
        perror("connect\n");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        //Read in
        char buffer[256];
        memset(buffer, 0, 256);
        fgets(buffer, 256, stdin);
        //Write to socket
        int n;
        if ((n = send(sockfd, buffer, strlen(buffer), MSG_NOSIGNAL)) < 0)
        {
            close(sockfd);
            perror("write\n");
            exit(EXIT_FAILURE);
        }

        //Read and print
        if (read(sockfd, buffer, 255) < 0)
        {
            perror("read\n");
            exit(EXIT_FAILURE);
        }

        printf("%s\n", buffer);
    }
    close(sockfd);
    exit(EXIT_SUCCESS);
}