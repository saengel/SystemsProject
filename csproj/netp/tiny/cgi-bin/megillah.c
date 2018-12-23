/*
 * adder.c - a minimal CGI program that adds two numbers together
 */
/* $begin adder */
#include "csapp.h"
#include "stdlib.h"
int main(void) {
    char *buf, *p;
    char arg1[MAXLINE], arg2[MAXLINE], content[MAXLINE];
    int n1=0, n2=0;

    /* Extract the two arguments passed in the url */

    // returning the value of the QUERY_STRING and save in buf
    //if ((buf = getenv("QUERY_STRING")) != NULL) {
    //  p = strchr(buf, '&'); // returns a pointer to the ampersand which separates in the url between the two parameters
    //  *p = '\0'; // replace the ampersand with \0 which is the string terminating character breaking it up into two strings
    //  strcpy(arg1, buf); // copy from buff into arg1 variable
    //  strcpy(arg2, p+1); // p is pointing to the ampersand, so p+1 points to the beginning of the second arg after the imported terminating char, and copy the second arg into arg2

	// translated from ascii number in string and converting it  to integer
    //	n1 = atoi(arg1);
    //	n2 = atoi(arg2);
    // }

    /* Make the response body */
    //sprintf() prints to a buffer instead of to the screen
    // way of generating a string with the entire response
    // aggregating response into a buffer called content
    // before printing it later (need to get length of entirety
    // of the content because needs to print before
    // in the header

    char * megillah;
    megillah = strncat(getenv("QUERY_STRING"), "\0", 1);
    
    sprintf(content, "Your seasonal megillah is <b> %s </b>", megillah);
    sprintf(content, "%sThanks for visiting!\r\n", content);


    if (strncmp(megillah, "aicha", 5) == 0){
      // Making a wget call to aicha
      // Saving it to aicha.txt.
      // TODO: Will need to generate random text file names so
      // multiple users can call without overwriting the system. 
      system("wget https://www.sefaria.org/api/texts/Lamentations.1 -O aicha.txt ");
     
      sprintf(content, "File downloaded from API successfully", content);
      // use rio to upload the strstr
      // Parse the result - strstr "text" ingest everything afterwards...
      // display the first perek to the user
    }
    else if (strncmp(megillah, "esther", 6) == 0) {
      // Making a wget call to esther
      // Saving it to esther.txt.                                                                                           
      // TODO: Will need to generate random text file names so                                             
      // multiple users can call without overwriting the system.                                                         
      system("wget https://www.sefaria.org/api/texts/Esther.1 -O esther.txt ");

      sprintf(content, "File downloaded from API successfully", content);
      // use rio to upload the result into a string
      // Parse the result
      // display the first perek to the user 
    }
    else if (strncmp(megillah, "kohelet", 7) == 0) {
      // Making a wget call to kohelet        
      // Saving it to kohelet.txt.                                                                            
      // TODO: Will need to generate random text file names so                                            
      // multiple users can call without overwriting the system.                                                         
      system("wget https://www.sefaria.org/api/texts/Ecclesiastes.1 -O kohelet.txt ");

      sprintf(content, "File downloaded from API successfully", content);
      // Make a wget call to kohelet - using system - generate new file name
      // open file then use rio, slide 25 in lesson 17     
      // use rio to upload the result into a string
      // Parse the result                                                   
      // display the first perek to the user                                   
    }
    else if (strncmp(megillah, "shir", 4) == 0) {
      // Making a wget call to Song_of_Songs              
      // Saving it to shir.txt.                        
      // TODO: Will need to generate random text file names so                  
      // multiple users can call without overwriting the system.                      
      system("wget https://www.sefaria.org/api/texts/Song_of_Songs.1 -O shir.txt ");

      sprintf(content, "File downloaded from API successfully", content);
      // use rio to upload the result into a string
      // open file then use rio, slide 25 in lesson 17
      // Parse the result                                                     
      // display the first perek to the user                                   
    }
    else if (strncmp(megillah, "rut", 3) == 0) {
      // Making a wget call to Ruth      
      // Saving it to rut.txt.                                                                                           
      // TODO: Will need to generate random text file names so                                          
      // multiple users can call without overwriting the system.                                                      
      system("wget https://www.sefaria.org/api/texts/Ruth.1 -O rut.txt ");

      sprintf(content, "File downloaded from API successfully", content);
      // use rio to upload the result into a string
      // Parse the result                                  
      // display the first perek to the user                                 
    }
    else{
      sprintf(content, "You requested <b> %s </b>\r\n", megillah, content);
      sprintf(content, "ERROR: %s is not a megillah\r\n", megillah, content);
    }
    
  
    /* Generate the HTTP response - which is just a bunch of print statements */

    // header of response - Going to be entirely different for our project. 
    printf("Connection: close\r\n");
    printf("Content-length: %d\r\n", (int)strlen(content));
    printf("Content-type: text/html\r\n\r\n");

    // body of response
    printf("%s", content); // printng the entirety of content to the screen
    fflush(stdout);

    exit(0);
}
/* $end adder */
