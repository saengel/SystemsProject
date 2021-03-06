/*
 * adder.c - a minimal CGI program that adds two numbers together
 */
/* $begin adder */
#include "csapp.h"

int main(void) {
    char *buf, *p;
    char arg1[MAXLINE], arg2[MAXLINE], content[MAXLINE];
    int n1=0, n2=0;

    /* Extract the two arguments passed in the url */
    if ((buf = getenv("QUERY_STRING")) != NULL) {
	p = strchr(buf, '&');
	*p = '\0';
	strcpy(arg1, buf);
	strcpy(arg2, p+1);

	// translated from ascii number in string and converting it  to integer
	n1 = atoi(arg1);
	n2 = atoi(arg2);
    }

    /* Make the response body */
    //sprintf() prints to a buffer instead of to the screen
    // way of generating a string with the entire response
    // aggregating response into a buffer called content
    // before printing it later (need to get length of entirety
    // of the content because needs to print before
    // in the header
    sprintf(content, "Welcome to Sarah's subtract.com: ");
    sprintf(content, "%sTHE Internet BASIC subtraction portal.\r\n<p>", content);
    sprintf(content, "%sThe amazing answer is: %d -  %d = %d\r\n<p>", 
	    content, n2, n1, n2-n1);
    sprintf(content, "%sThanks for visiting!\r\n", content);
  
    /* Generate the HTTP response - which is just a bunch of print statements */

    // header of response
    printf("Connection: close\r\n");
    printf("Content-length: %d\r\n", (int)strlen(content));
    printf("Content-type: text/html\r\n\r\n");

    // body of response
    printf("%s", content); // printng the entirety of content to the screen
    fflush(stdout);

    exit(0);
}
/* $end adder */
