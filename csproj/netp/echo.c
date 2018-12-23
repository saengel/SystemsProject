/*
 * echo - read and echo text lines until client closes connection
 */
/* $begin echo */
#include "csapp.h"

void echo(int connfd) 
{
    size_t n; 
    char buf[MAXLINE]; // Setting up the string of characters 
    rio_t rio;

    // Reads
    Rio_readinitb(&rio, connfd);

    // Loop where it reads from the connfd which is the connection the server
    // received from accept. When the client writes on the client side connection file descriptor
    // it appears for the server on connfd. 
    while((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) { //line:netp:echo:eof
        printf("server received %d bytes from the string: %s \n", (int)n, buf);
	Rio_writen(connfd, buf, n);
    }
}
/* $end echo */

