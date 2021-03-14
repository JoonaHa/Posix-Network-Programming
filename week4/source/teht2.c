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

    if (argc != 2)
    {
        fprintf(stderr, "usage: %s hostname\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    //Get server port
    service_name = getservbyname("echo", "tcp");
    if (service_name == NULL)
    {
        fprintf(stderr, "%s: no echo service on this host\n", argv[0]);
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
    sa.sin_port = service_name->s_port;
    sa.sin_family = host_name->h_addrtype;
    bcopy(host_name->h_addr, &sa.sin_addr.s_addr, host_name->h_length);
    char *ip = inet_ntoa(sa.sin_addr);
    printf("Host adress: %s Ip adress: %s", host_name->h_addr, ip);

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
        if (write(sockfd, buffer, strlen(buffer)) < 0)
        {
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

    exit(EXIT_SUCCESS);
}