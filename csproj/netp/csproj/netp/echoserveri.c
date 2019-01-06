/* 
 * echoserveri.c - An iterative echo server 
 */ 
/* $begin echoserverimain */
#include "csapp.h"

void echo(int connfd);

int main(int argc, char **argv) 
{
    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;  /* Enough space for any address */  //line:netp:echoserveri:sockaddrstorage
    char client_hostname[MAXLINE], client_port[MAXLINE];

    // Checking for the right number of args
    if (argc != 2) {
	fprintf(stderr, "usage: %s <port>\n", argv[0]);
	exit(0);
    }

    // Open up the listenfd file descriptor
    // Opening based on the only necessary argument which is the port
    listenfd = Open_listenfd(argv[1]);

    // Infinite listening loop - server needs to be listening
    while (1) {
	clientlen = sizeof(struct sockaddr_storage);
	// Calling Accept() which will block until the client shows up
	connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
	// Printing the IP address the client connected from
        Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAXLINE, 
                    client_port, MAXLINE, 0);
        printf("Connected to (%s, %s)\n", client_hostname, client_port);

	// Echo defined before main, defined in a different file
	// Reference above main 
	echo(connfd);
	Close(connfd);
    }
    exit(0);
}
/* $end echoserverimain */
