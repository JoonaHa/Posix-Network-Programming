#include	"unp.h"
#include <unistd.h>
int
main(int argc, char **argv)
{
	int					sockfd, n;
	char				recvline[MAXLINE + 1];
	struct sockaddr_in	servaddr;
	char*				end;
	unsigned int 		port, nmbrcontections;


	if ((argc != 4) || (port = strtoul(argv[2], &end, 0)) < 0 || (nmbrcontections = strtoul(argv[3], &end, 0)) < 0)
	{
        err_quit("usage: a.out <IPaddress> <Port> <Connections>");
    }
	printf("PORT: %d HTONS %d\n", port, htons(port));

	for (int i = 0; i < nmbrcontections; i++)
	{
		sleep(1);
		if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			err_sys("socket error");

		bzero(&servaddr, sizeof(servaddr));
		servaddr.sin_family = AF_INET;
		servaddr.sin_port   = htons(port);	/* daytime server */
		if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
			err_quit("inet_pton error for %s", argv[1]);

		if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
			err_sys("connect error");

		while ( (n = read(sockfd, recvline, MAXLINE)) > 0) {
			recvline[n] = 0;	/* null terminate */
			if (fputs(recvline, stdout) == EOF)
				err_sys("fputs error");
		}
		if (n < 0)
			err_sys("read error");
	}	

	exit(0);
}
