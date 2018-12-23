/*
 * echoclient.c - An echo client
 * This is my call to Sefaria (but will eventually copy this all into one file 
* KEEP ALL CODE - JUST FIGURE OUT THE HOST, PORT AND BUFF PROPER SYNTAX FOR THE GET REQUEST
/* $begin echoclientmain */
#include "csapp.h"

int main(int argc, char **argv) 
{
    int clientfd;
    char *host, *port, buf[MAXLINE];
    rio_t rio;

    // Create a BUF explicitly to form the request
    // hardcode the buf request based on which month the user
    // chooses.

    // BASED ON WHAT USER CHOOSES, MAKE A WGET? But no socket even.... 

    // Testing for error in command line arguments
    // In this case taking two args in addition to the command = 3 argcs
    // Will probably be hardcoded in our case
    if (argc != 3) {
	fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
	exit(0);
    }

    // Saving the command line arguments
    // will somehow have to get the desired megillah from the user's html button choices
    host = "www.sefaria.org/api/texts/Esther"; // FIGURE OUT EXACTLY WHAT THE HOST IS 
    port = "80"; // might not be 80

    // File descriptors are the same irregardless of whether or not
    // it's a connection, or printer etc.
    // Open_clientfd() is a wrapper around connect
    clientfd = Open_clientfd(host, port);
    Rio_readinitb(&rio, clientfd);

    // Fgets = get string
    // Reading from standard in (aka reading char from keyboard)
    // and writing them to client_fd which is the network connection
    // 
    while (Fgets(buf, MAXLINE, stdin) != NULL) {
      Rio_writen(clientfd, buf, strlen(buf)); // Writes to client_fd which is the network con
      Rio_readlineb(&rio, buf, MAXLINE);      // Reads it from the network
      Fputs(buf, stdout);                     // Puts the string on network to standard out
    }
    Close(clientfd); //line:netp:echoclient:close
    exit(0);
}
/* $end echoclientmain */
