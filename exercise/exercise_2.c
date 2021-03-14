#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <errno.h>
#include <wait.h>
#include <ctype.h>
#include <signal.h>

#define SERVICE "UNIX_TL"
#define HOST "whx-10.cs.helsinki.fi"
#define MAXBUFFER 2048
const char *SRCPATH;

void handle_signals(int sig);
int filetransfer(int sockfd, char *buffer, struct in_addr clientIp);
int getsrcfldr(int sockfd, int logfd);
int echoer(char line[], int size, int sockfd, int logfd);
int newConnection(int sockfd, int logfd, int respfd, struct in_addr clientIp);
int childServer(int port, int logfd, int respfd);
char **split(char *read, char *separator);

/* Signal handler just for closing all child processes in case of SIGINT or SIGPIPE 
*/
void handle_signals(int sig)
{
    char message[64] = "Caugh signal:";

    if (sig == SIGPIPE)
        strcat(message, "SIGPIPE\n");

    if (sig == SIGINT)
        strcat(message, "SIGINT\n");

    write(STDOUT_FILENO, message, strlen(message));
    wait(0);
    exit(EXIT_SUCCESS);
}

int main(int argc, char **argv)
{

    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <relative path to source>\n", argv[0]);

        exit(EXIT_FAILURE);
    }

    // Set signalhandler
    struct sigaction act;
    act.sa_handler = handle_signals;
    sigaction(SIGPIPE, &act, NULL);
    sigaction(SIGINT, &act, NULL);

    // set SRCPATH constant to sourcefile's relative path
    SRCPATH = argv[1];

    int sockfd, pid;
    struct sockaddr_in sa;
    struct servent *service_name;
    struct hostent *host_name;

    //Get server port
    service_name = getservbyname(SERVICE, "tcp");
    if (service_name == NULL)
    {
        fprintf(stderr, "%s: no service on this host\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    //Get address from hostname
    if ((host_name = gethostbyname(HOST)) == NULL)
    {
        fprintf(stderr, "%s: cannot find host %s\n", argv[0], HOST);
        exit(EXIT_FAILURE);
    }
    printf("Service port: %d\n", service_name->s_port);

    //Copy port and ip-address to sockaddr_in
    memset(&sa, 0, sizeof(sa));

    sa.sin_family = host_name->h_addrtype;
    sa.sin_port = service_name->s_port;
    bcopy(host_name->h_addr, &sa.sin_addr.s_addr, host_name->h_length);

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

    int read_length, port, n;

    /* Log files for connection responses and overall logging
    */
    int filefd = open("./exercise.log", O_CREAT | O_WRONLY, 0666);
    int respfd = open("./response.log", O_CREAT | O_WRONLY, 0666);
    char buffer[MAXBUFFER + 1];
    while ((read_length = read(sockfd, buffer, MAXBUFFER)) > 0)
    {

        //Hardcoded answes for first questions
        if (n == 0)
        {

            char greetings[] = "HELLO\n";
            write(sockfd, greetings, strlen(greetings));
            write(STDOUT_FILENO, greetings, strlen(greetings));
        }
        if (n == 1)
        {
            char loginname[] = "joonahal\n";
            write(sockfd, loginname, strlen(loginname));
            write(STDOUT_FILENO, loginname, strlen(loginname));
        }
        if (n == 4)
        {
            char **result = split(buffer, " ");
            port = (int)atol(result[11]);
            printf("\n PORT: %d \n", port);

            // Error
            if ((pid = fork()) < 0)
            {
                perror("fork\n");
                exit(EXIT_FAILURE);
            }
            // Child fork
            else if (pid == 0)
            {
                close(sockfd);
                childServer(port, filefd, respfd);

                exit(EXIT_SUCCESS);
            }
            else
            {
                printf("Fork server to port: %d\n", port);
            }
        }
        write(STDOUT_FILENO, buffer, strlen(buffer));

        n++;
        memset(buffer, 0, MAXBUFFER);
    }

    if (read_length < 0)
        perror("read\n");

    wait(0);
    exit(EXIT_SUCCESS);
}

// Below are functions for server and handling commads and new connections
int filetransfer(int sockfd, char *buffer, struct in_addr clientIp)
{
    int filetransferfd, bytes;
    long filesize;
    struct sockaddr_in cliaddr;

    //parase port and filepath form receivedbuffer
    char path[MAXBUFFER + 1];
    char **result = split(buffer, " ");
    strcpy(path, result[1]);
    int port = (int)atol(result[2]);
    printf("Filetransfer port :%d\n", port);

    //Connect to the give port
    memset(&cliaddr, 0, sizeof(cliaddr));
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(port);
    cliaddr.sin_addr = clientIp;
    if ((filetransferfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        fprintf(stderr, "Filetransfer socket error Errno: %d\n", errno);
        exit(EXIT_FAILURE);
    }
    if (connect(filetransferfd, (struct sockaddr *)&cliaddr, sizeof(cliaddr)) < 0)
    {
        fprintf(stderr, "Filetransfer connect errno:%d\n", errno);
        exit(EXIT_FAILURE);
    }

    //Open file
    FILE *file = fopen(path, "r");

    if (file == NULL)
    {
        printf("File Not Found!\n");
        return (EXIT_FAILURE);
    }
    //Calculate file size
    fseek(file, 0L, SEEK_END);
    char sendbuf[MAXBUFFER + 1];
    memset(sendbuf, 0, MAXBUFFER + 1);
    filesize = (long)ftell(file);
    sprintf(sendbuf, "%ld", filesize);
    strcat(sendbuf, "\n");

    //write to filesize to original socket and log
    if ((bytes = write(sockfd, sendbuf, strlen(sendbuf))) < 0)
    {
        dprintf(STDERR_FILENO, "Read failed errno: %d \n", errno);
        return (EXIT_FAILURE);
    }


    // go to beginning of the file and copy it to filebuf
    fseek(file, 0L, SEEK_SET);
    char filebuf[filesize + 1];
    memset(filebuf, 0, filesize + 1);
    char ch;
    int i;
    for (i = 0; (ch == EOF || i < filesize); i++)
    {

        ch = fgetc(file);
        filebuf[i] = ch;
    }

    //write to the new socket
    int len = i;
    printf("Filetransfer read %d bytes\n", len);

    write(filetransferfd, filebuf, len);

    fclose(file);
    close(filetransferfd);
    return (EXIT_SUCCESS);
}

int getsrcfldr(int sockfd, int logfd)
{
    int bytes;
    char buffer[MAXBUFFER];
    strcpy(buffer, SRCPATH);
    strcat(buffer, "\n");

    //write relative path

    if ((bytes = write(sockfd, buffer, strlen(buffer))) <= 0)
    {
        dprintf(STDERR_FILENO, "Write failed errno: %d \n", errno);
        dprintf(logfd, "Write failed errno: %d \n", errno);
        return (EXIT_FAILURE);
    }

    write(logfd, buffer, strlen(buffer));
    return (EXIT_SUCCESS);
}

int echoer(char line[], int size, int sockfd, int logfd)
{
    int bytes;
    char buffer[MAXBUFFER + 1];
    strncpy(buffer, line, size);
    //lopp for changing chracters to uppercase
    for (int i = 0; i < sizeof(buffer); i++)
    {
        if (isupper(buffer[i]))
            continue;
        else
            buffer[i] = toupper(buffer[i]);
    }

    // write to socket and log
    if ((bytes = write(sockfd, buffer, size)) <= 0)
    {
        dprintf(STDERR_FILENO, "Write failed errno: %d \n", errno);
        dprintf(logfd, "Write failed errno: %d \n", errno);
        return (EXIT_FAILURE);
    }

    write(logfd, buffer, size);
    return (EXIT_SUCCESS);
}

int newConnection(int sockfd, int logfd, int respfd, struct in_addr clientIp)
{

    // Ebuffer is for saving multi message E commands
    int read_len;
    char buffer[MAXBUFFER + 1];
    int line = 0;
    int esize = 0;
    char ebuffer[MAXBUFFER + 1];
    while ((read_len = read(sockfd, buffer, MAXBUFFER)) > 0)
    {
        // write everyline to respfd
        write(respfd, buffer, read_len);

        if (buffer[0] == 'E')
        {
            // If last read was E command and this read is also E: echo last read
            if (esize > 0)
            {
                echoer(ebuffer, (esize + read_len), sockfd, logfd);
                memset(ebuffer, 0, sizeof(ebuffer));
                esize = 0;
            }

            esize = read_len;
            strcpy(ebuffer, buffer);
            dprintf(logfd, "\nFOUND E!!!!\n");
            write(logfd, buffer, read_len);
        }

        else if (buffer[0] == '#')
        {
            dprintf(logfd, "\nComment line:%d \n", line++);
            dprintf(logfd, "\n%s\n", buffer);

            // Close program if comment is goodbye
            if (strstr(buffer, "209") != NULL)
            {

                printf("Found Goodbye. Closing the programn\n");
                kill(getppid(), SIGTERM);
                return (EXIT_SUCCESS);
            }
        }

        else if (buffer[0] == 'C')
        {
            dprintf(logfd, "\nFOUND C!!!!\n");
            write(logfd, buffer, read_len);
            getsrcfldr(sockfd, logfd);
        }

        else if (buffer[0] == 'F')
        {
            dprintf(logfd, "\nF\n");
            dprintf(logfd, "\n%s\n", buffer);
            filetransfer(sockfd, buffer, clientIp);
        }

        else if (buffer[0] == 'A')
        {
            //Accepted terminate program
            dprintf(logfd, "\nA\n");
            printf("Found Accept. Closing the programn\n");
            shutdown(O_RDWR, sockfd);
            kill(getppid(), SIGTERM);
            return (EXIT_SUCCESS);
        }

        else if (buffer[0] == 'Q')
        {
            dprintf(logfd, "\nQ\n");
            shutdown(O_RDWR, sockfd);
            close(sockfd);
            return (EXIT_SUCCESS);
        }
        else
        {
            /*
            If last read was E command and this read is not a command and comment: 
            append it to ebuffer and echo it
            */
            if (esize > 0)
            {

                memcpy(&ebuffer[esize], buffer, read_len);
                echoer(ebuffer, (esize + read_len), sockfd, logfd);
                memset(ebuffer, 0, sizeof(ebuffer));
                esize = 0;
            }
            dprintf(logfd, "\nSOMETHING ELSE\n");
            dprintf(logfd, "\n%s\n", buffer);
        }

        memset(buffer, 0, MAXBUFFER);
    }

    if (read_len < 0)
        dprintf(logfd, "Read failed errno: %d \n", errno);
    return (EXIT_FAILURE);
}

int childServer(int port, int logfd, int respfd)
{
    int listenfd, sockfd, forkpid, clilen;
    struct sockaddr_in serv_addr, cli_addr;

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

    // Bind socket and set it passive mode

    if (bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        fprintf(stderr, "Error binding local address Errno: %d \n", errno);
        exit(EXIT_FAILURE);
    }
    listen(listenfd, 20);

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

        if ((forkpid = fork()) < 0)
        {
            fprintf(stderr, "Fork failed errno: %d \n", errno);
        }
        //Child process
        else if (forkpid == 0)
        {
            close(listenfd);
            char ipbuffer[500];
            inet_ntop(AF_INET, &cli_addr.sin_addr, ipbuffer, 500);
            printf("\nIP: %s\n", ipbuffer);
            struct in_addr clientIp = cli_addr.sin_addr;
            newConnection(sockfd, logfd, respfd, clientIp);
            return (EXIT_SUCCESS);
        }
        //Parent
        else
        {
            close(sockfd);
            printf("Parent PID: %d\n", getpid());
            printf("Connection PID:%d\n", forkpid);
        }
    }

    wait(0);
    return (EXIT_SUCCESS);
}

// Utility funbction for splitting strings
char **split(char *read, char *separator)
{

    char **result = 0;
    size_t count = 0;
    char line[256];
    strcpy(line, read);
    size_t idx = 0;
    char *tmp = line;
    char delimeter[2];
    strcpy(delimeter, separator);
    char *last_delimiter = 0;

    while (*tmp)
    {
        if (delimeter[0] == *tmp)
        {
            count++;
            last_delimiter = tmp;
        }
        tmp++;
    }

    count += last_delimiter < (line + strlen(line) - 1);

    count++;

    result = malloc(sizeof(char *) * count);

    if (result)
    {
        char *token = strtok(line, delimeter);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delimeter);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}
