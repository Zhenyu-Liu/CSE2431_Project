/* getfile server that handles only one client at a time */
#include <stdio.h> /* printf and standard i/o */
#include <sys/socket.h> /* socket, bind, listen, accept, socklen_t */
#include <arpa/inet.h> /* sockaddr_in, inet_ntop */
#include <string.h> /* strlen */
#include <stdlib.h> /* atoi, EXIT_FAILURE */
#include <fcntl.h> /* open, O_RDONLY */
#include <unistd.h> /* close, read */
#define SRV_PORT 5103 /* default port number */
#define MAX_RECV_BUF 256
#define MAX_SEND_BUF 256
char* SRV_IP = "10.0.2.15";
void get_file_name(int, char*);
int send_file(int , char*);

int main(int argc, char* argv[])
{
 int sock_fd;
 struct sockaddr_in srv_addr;
 char file_name [MAX_RECV_BUF]; /* name of the file to be sent */

 memset(&srv_addr, 0, sizeof(srv_addr)); /* zero-fill srv_addr structure*/

 /* create a client socket */
 sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
 srv_addr.sin_family = AF_INET; /* internet address family */

 /* convert command line argument to numeric IP */
 if ( inet_pton(AF_INET, SRV_IP, &(srv_addr.sin_addr)) < 1 )
 {
 printf("Invalid IP address\n");
exit(EXIT_FAILURE);
 }

 /* if port number supplied, use it, otherwise use SRV_PORT */
 srv_addr.sin_port = (argc > 1) ? htons(atoi(argv[1])) : htons(SRV_PORT);

 if( connect(sock_fd, (struct sockaddr*) &srv_addr, sizeof(srv_addr)) < 0 )
 {
 perror("connect error");
 exit(EXIT_FAILURE);
 }
 printf("connected to:%s:%d ..\n",SRV_IP,SRV_PORT);

 get_file_name(sock_fd, file_name);
 send_file(sock_fd, file_name);

 /* close socket*/
 if(close(sock_fd) < 0)
 {
    perror("socket close error");
 exit(EXIT_FAILURE);
 }
 return 0;
}


void get_file_name(int sock, char* file_name)
{
 char recv_str[MAX_RECV_BUF]; /* to store received string */
 ssize_t rcvd_bytes; /* bytes received from socket */

 /* read name of requested file from socket */
 if ( (rcvd_bytes = recv(sock, recv_str, MAX_RECV_BUF, 0)) < 0) {
 perror("recv error");
 return;
 }

 sscanf (recv_str, "%s\n", file_name); /* discard CR/LF */
}

int send_file(int sock, char *file_name)
{
 int sent_count; /* how many sending chunks, for debugging */
 ssize_t read_bytes, /* bytes read from local file */
 sent_bytes, /* bytes sent to connected socket */
 sent_file_size;
 char send_buf[MAX_SEND_BUF]; /* max chunk size for sending file */
 char * errmsg_notfound = "File not found\n";
 int f; /* file handle for reading local file*/
 sent_count = 0;
 sent_file_size = 0;
 /* attempt to open requested file for reading */
 if( (f = open(file_name, O_RDONLY)) < 0) /* can't open requested file */
 {
   perror(file_name);
   if( (sent_bytes = send(sock, errmsg_notfound ,
                          strlen(errmsg_notfound), 0)) < 0 )
     {
       perror("send error");
       return -1;
     }
 }
 else /* open file successful */
   {
     printf("Sending file: %s\n", file_name);
     while( (read_bytes = read(f, send_buf, MAX_RECV_BUF)) > 0 )
       {
         if( (sent_bytes = send(sock, send_buf, read_bytes, 0))
             < read_bytes )
           {
             perror("send error");
             return -1;
           }
         sent_count++;
         sent_file_size += sent_bytes;
       }
     close(f);
   } /* end else */

 printf("Done with this client. Sent %d bytes in %d send(s)\n\n",
        sent_file_size, sent_count);
 return sent_count;
}