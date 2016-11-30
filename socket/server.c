/* getfile server that handles only one client at a time */
#include <stdio.h> /* printf and standard i/o */
#include <sys/socket.h> /* socket, bind, listen, accept, socklen_t */
#include <arpa/inet.h> /* sockaddr_in, inet_ntop */
#include <string.h> /* strlen */
#include <stdlib.h> /* atoi, EXIT_FAILURE */
#include <fcntl.h> /* open, O_RDONLY */
#include <unistd.h> /* close, read */
#define SRV_PORT 5103 /* default port number */
#define LISTEN_ENQ 5 /* for listen backlog */
#define MAX_RECV_BUF 256
#define MAX_SEND_BUF 256
int recv_file(int,char*);

int main(int argc, char* argv[])
{
        int listen_fd, conn_fd;
        struct sockaddr_in srv_addr, cli_addr;
        socklen_t cli_len;
        char print_addr [INET_ADDRSTRLEN]; /* readable IP address */
        if (argc < 2) {
                printf("usage: %s <filename> [port number]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
        memset(&srv_addr, 0, sizeof(srv_addr)); /* zero-fill srv_addr structure*/
        memset(&cli_addr, 0, sizeof(cli_addr)); /* zero-fill cli_addr structure*/
        srv_addr.sin_family = AF_INET;
        srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);;
        /* if port number supplied, use it, otherwise use SRV_PORT */
        srv_addr.sin_port = (argc > 2) ? htons(atoi(argv[2])) : htons(SRV_PORT);
        if ( (listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
                perror("socket error");
                exit(EXIT_FAILURE);
        }
        /* bind to created socket */
        if( bind(listen_fd, (struct sockaddr*) &srv_addr, sizeof(srv_addr)) < 0 ) {
                perror("bind error");
                exit(EXIT_FAILURE);
        }

        printf("Listening on port number %d ...\n", ntohs(srv_addr.sin_port));
        if( listen(listen_fd, LISTEN_ENQ) < 0 ) {
                perror("listen error");
                exit(EXIT_FAILURE);
        }

        for(;; ) /* run forever*/
        {
                cli_len = sizeof(cli_addr);
                printf ("Waiting for a client to connect...\n\n");
                /* block until some client connects */
                if ( (conn_fd = accept(listen_fd, (struct sockaddr*) &cli_addr,
                                       &cli_len)) < 0 )
                {
                        perror("accept error");
                        break; /* exit from the for loop */
                }

                /* convert numeric IP to readable format for displaying */
                inet_ntop(AF_INET, &(cli_addr.sin_addr), print_addr, INET_ADDRSTRLEN);
                printf("Client connected from %s:%d\n",
                       print_addr, ntohs(cli_addr.sin_port) );

                recv_file(conn_fd, argv[1]); /* argv[1] = file name */
                printf("Closing connection\n");
                close(conn_fd); /* close connected socket*/
        } /* end for */

        close(listen_fd); /* close listening socket*/
        return 0;
}

int recv_file(int sock, char* file_name)
{
        char send_str [MAX_SEND_BUF]; /* message to be sent to server*/
        int f; /* file handle for receiving file*/
        ssize_t sent_bytes, rcvd_bytes, rcvd_file_size;
        int recv_count; /* count of recv() calls*/
        char recv_str[MAX_RECV_BUF]; /* buffer to hold received data */
        size_t send_strlen; /* length of transmitted string */

        sprintf(send_str, "%s\n", file_name); /* add CR/LF (new line) */
        send_strlen = strlen(send_str); /* length of message to be transmitted */
        if( (sent_bytes = send(sock, file_name, send_strlen, 0)) < 0 ) {
                perror("send error");
                return -1;
        }
        /* attempt to create file to save received data. 0644 = rw-r--r-- */
        if ( (f = open(file_name, O_WRONLY|O_CREAT, 0644)) < 0 )
        {
                perror("error creating file");
                return -1;
        }

        recv_count = 0; /* number of recv() calls required to receive the file */
        rcvd_file_size = 0; /* size of received file */

        /* continue receiving until ? (data or close) */
        while ( (rcvd_bytes = recv(sock, recv_str, MAX_RECV_BUF, 0)) > 0 )
        {
                recv_count++;
                rcvd_file_size += rcvd_bytes;

                if (write(f, recv_str, rcvd_bytes) < 0 )
                {
                        perror("error writing to file");
                        return -1;
                }
        }
        close(f); /* close file*/
        printf("Client Received: %d bytes in %d recv(s)\n", rcvd_file_size,
               recv_count);
        return rcvd_file_size;
}
