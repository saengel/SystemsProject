/*
 * megillah.c - a CGI program which returns the first chapter
 * of the most seasonally relevant megillah using the 
 * Sefaria API and user input.
 */
/* $begin megillah */
#include "csapp.h"
#include "stdlib.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include "jsmn/jsmn.h"


/*Function for JSMN taken from documentation*/
static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
  if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
      strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
    return 0;
  }
  return -1;
}

int main(void) {
  // char *buf, *p;
  //  rio_t rio;
    char megillahBuf[35000];
    char content[MAXLINE*10];
    
    char * megillah;
    megillah = strncat(getenv("QUERY_STRING"), "\0", 1);
    sprintf(content,"%s <html> <style>  div.main{ text-align:center; color:white; font-family: Helvetica, sans-serif;  text-decoration: none;}.button {background-color:lightblue: border-color: white;border-width: thick;color: DodgerBlue;padding: 15px 32px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer;} .button:hover {background-color: white; color: #FFDF80;}</style><div class = \"main\"><head><title>Megillot!</title></head><body style=\"background-color:DodgerBlue;\">", content);
    
    if (strncmp(megillah, "aicha", 5) == 0){
      sprintf(content, "%s<p><i>Our algorithm has determined that appropriate seasonal learning for you is...</i></p><h1><u>Aicha</u>: Chapter One</h1>", content);
      // Making a wget call to aicha and saving it to aicha.txt. 
      int result = system("wget https://www.sefaria.org/api/texts/Lamentations.1 -O aicha.txt ");
      // Opening API into string    
      int fd;
      fd = open("/home/sengel/csproj/netp/tiny/aicha.txt", O_RDONLY, 0);
      if (fd < 0){
        sprintf(content, "%s<br>Open failing<br>\r\n", content);
      }
      
      // FIX TO NOT BE HARDCODED                                
      int size = 30042;   
                      
      int status;
      status = rio_readn(fd, megillahBuf, size);   
      if (status == -1){               
	sprintf(content, "%sRead Error<br>\r\n", content);   
      }
      else if (status == 0){                            
	sprintf(content, "%sEOF zero returned from read<br>\r\n", content);
      }
      else{                   
        //sprintf(content, "%sSuccess. %d bytes transferred in read<br>\r\n", content, status);                
        //Prints the API to the screen....  
        //sprintf(content, "%s%s", content, megillahBuf);

	// JSMN ATTEMPT
	jsmn_parser p;
	jsmntok_t t[600]; /* We expect no more than 600 tokens */
	int r;
	
	jsmn_init(&p);
	r = jsmn_parse(&p, megillahBuf, strlen(megillahBuf), t, sizeof(t)/sizeof(t[0]));
	if (r < 0) {
	  sprintf(content, "%sFailed to parse JSON: %d\n", content, r);
	  //return 1;
	}
	
	
	/* Assume the top-level element is an object */
	if (r < 1 || t[0].type != JSMN_OBJECT) {
	  sprintf(content, "%s<br>Object expected\n", content);
	  //return 1;
	}

	int i;
	
	/* Loop over all keys of the root object */
	for (i = 1; i < r; i++) {
	  if (jsoneq(megillahBuf, &t[i], "text") == 0) {
	    /* We expect text to be an array of strings */
	   int j;
	   printf("- Text:\n<br>");
	   if (t[i+1].type != JSMN_ARRAY) {
	     continue; /* We expect groups to be an array of strings */
	   }
	   for (j = 0; j < t[i+1].size; j++) {
	     jsmntok_t *g = &t[i+j+2];
	     sprintf(content, "%s  <br><br> %.*s\n", content, g->end - g->start, megillahBuf + g->start);
	   }
	   i += t[i+1].size + 1;
	  }
	  else { 
	    sprintf(content, "%s<!--Unexpected key: %.*s\n-->", content, t[i].end-t[i].start, megillahBuf + t[i].start);
	  }
	  
	}

	// Trying to print the error to the screen
	fprintf(stderr, "%s");
	
      }

    }
    else if (strncmp(megillah, "esther", 6) == 0) {
      // Making a wget call to esther and saving it to esther.txt                            
      int result = system("wget https://www.sefaria.org/api/texts/Esther.1 -O esther.txt ");
      //sprintf(content, "%sFile downloaded from API successfully<br>\r\n", content);
      sprintf(content, "%s<p><i>Our algorithm has determined that appropriate seasonal learning for you is...</i></p><h1><u>Esther</u>: Chapter One</h1>", content); 
      // use rio to upload the result into a string
      int fd;
      fd = open("/home/sengel/csproj/netp/tiny/esther.txt", O_RDONLY, 0);
      
      if (fd < 0){
	sprintf(content, "%s<br>Open failing<br>\r\n", content);
      }
      
      // FIX TO NOT BE HARDCODED
      int size = 31127;
      int status;
      status = rio_readn(fd, megillahBuf, size);
      
      if (status == -1){
	sprintf(content, "%sRead Error\r\n", content);
      }
      else if (status == 0){
	sprintf(content, "%sEOF zero returned from read\r\n", content);
      }
      else{
//sprintf(content, "%sSuccess. %d bytes transferred in read<br>\r\n", content, status);                                                                                      
        //Prints the API to the screen....                                                                                                                                           
        //sprintf(content, "%s%s", content, megillahBuf);                                                                                                                            

        // JSMN ATTEMPT                                                                                                                                                              
        jsmn_parser p;
        jsmntok_t t[600]; /* We expect no more than 600 tokens */
        int r;

        jsmn_init(&p);
        r = jsmn_parse(&p, megillahBuf, strlen(megillahBuf), t, sizeof(t)/sizeof(t[0]));
        if (r < 0) {
          sprintf(content, "%sFailed to parse JSON: %d\n", content, r);
          //return 1;                                                                                                                                                                
        }


        /* Assume the top-level element is an object */
        if (r < 1 || t[0].type != JSMN_OBJECT) {
          sprintf(content, "%s<br>Object expected\n", content);
          //return 1;                                                                                                                                                                
        }

        int i;

        /* Loop over all keys of the root object */
        for (i = 1; i < r; i++) {
          if (jsoneq(megillahBuf, &t[i], "text") == 0) {
            /* We expect text to be an array of strings */
           int j;
           printf("- Text:\n<br>");
           if (t[i+1].type != JSMN_ARRAY) {
             continue; /* We expect groups to be an array of strings */
           }
           for (j = 0; j < t[i+1].size; j++) {
             jsmntok_t *g = &t[i+j+2];
             sprintf(content, "%s  <br><br> %.*s\n", content, g->end - g->start, megillahBuf + g->start);
           }
           i += t[i+1].size + 1;
          }
          else {
            sprintf(content, "%s<!--Unexpected key: %.*s\n-->", content, t[i].end-t[i].start, megillahBuf + t[i].start);
          }
	  
        }
	
        // Trying to print the error to the screen                                                                                                                                   
        fprintf(stderr, "%s");
	
	
      }
    }
    else if (strncmp(megillah, "kohelet", 7) == 0) {
      // Making a wget call to kohelet and saving it to kohelet.txt.                                        
      int result = system("wget https://www.sefaria.org/api/texts/Ecclesiastes.1 -O kohelet.txt ");
      sprintf(content, "%s<p><i>Our algorithm has determined that appropriate seasonal learning for you is...</i></p><h1><u>Kohelet</u>: Chapter One</h1>", content); 
      
      // use rio to upload the result into a string 
      int fd;
      fd = open("/home/sengel/csproj/netp/tiny/kohelet.txt", O_RDONLY, 0);
      if (fd < 0){
        sprintf(content, "%s<br>Open failing<br>\r\n", content);   
        // Seeing the error message                       
        char * message = strerror(errno);
	sprintf(content, "%s<br>ERROR MESSAGE:<br>%s\r\n", content, message);
      }
      
      // FIX TO NOT BE HARDCODED                    
      int size = 19253;                               
      int status;
      status = rio_readn(fd, megillahBuf, size);   
      if (status == -1){                                      
	sprintf(content, "%sRead Error", content);                      
      }
      else if (status == 0){                                                 
	sprintf(content, "%sEOF zero returned from read", content);
      }
      else {
	//sprintf(content, "%sSuccess. %d bytes transferred in read<br>\r\n", content, status);                                                                                      
        //Prints the API to the screen....                                                                                                                                           
        //sprintf(content, "%s%s", content, megillahBuf);                                                                                                                            
	
        // JSMN ATTEMPT                                                                                                                                                              
        jsmn_parser p;
        jsmntok_t t[600]; /* We expect no more than 600 tokens */
        int r;
	
        jsmn_init(&p);
        r = jsmn_parse(&p, megillahBuf, strlen(megillahBuf), t, sizeof(t)/sizeof(t[0]));
        if (r < 0) {
          sprintf(content, "%sFailed to parse JSON: %d\n", content, r);
          //return 1;                                                                                                                                                                
        }
	
	
        /* Assume the top-level element is an object */
        if (r < 1 || t[0].type != JSMN_OBJECT) {
          sprintf(content, "%s<br>Object expected\n", content);
          //return 1;                                                                                                                                                                
        }
	
        int i;

        /* Loop over all keys of the root object */
        for (i = 1; i < r; i++) {
          if (jsoneq(megillahBuf, &t[i], "text") == 0) {
            /* We expect text to be an array of strings */
	    int j;
           printf("- Text:\n<br>");
           if (t[i+1].type != JSMN_ARRAY) {
             continue; /* We expect groups to be an array of strings */
           }
           for (j = 0; j < t[i+1].size; j++) {
             jsmntok_t *g = &t[i+j+2];
             sprintf(content, "%s  <br><br> %.*s\n", content, g->end - g->start, megillahBuf + g->start);
           }
           i += t[i+1].size + 1;
          }
          else {
            sprintf(content, "%s<!--Unexpected key: %.*s\n-->", content, t[i].end-t[i].start, megillahBuf + t[i].start);
          }
	  
        }
	
        // Trying to print the error to the screen                                                                                                                                   
        fprintf(stderr, "%s");
	
	
      }
    }
    else if (strncmp(megillah, "shir", 4) == 0) {
      // Making a wget call to Song_of_Songs and saving it to shir.txt.          
      int result = system("wget https://www.sefaria.org/api/texts/Song_of_Songs.1 -O shir.txt ");
      sprintf(content, "%s<p><i>Our algorithm has determined that appropriate seasonal learning for you is...</i></p><h1><u>Shir HaShirim</u>: Chapter One</h1>", content); 
      // use rio to upload the result into a string             
      int fd;
      fd = open("/home/sengel/csproj/netp/tiny/shir.txt", O_RDONLY, 0);
      if (fd < 0){
        sprintf(content, "%s<br>Open failing<br>\r\n", content);
      }
      
      // FIX TO NOT BE HARDCODED                                                  
      int size = 17103;   
                                                         
      int status;
      status = rio_readn(fd, megillahBuf, size);   
      if (status == -1){                                                                  
	sprintf(content, "%sRead Error", content);                                
      }
      else if (status == 0){                                
	sprintf(content, "%sEOF zero returned from read", content);
      } else{
//sprintf(content, "%sSuccess. %d bytes transferred in read<br>\r\n", content, status);                                                                                      
        //Prints the API to the screen....                                                                                                                                           
        //sprintf(content, "%s%s", content, megillahBuf);                                                                                                                            

        // JSMN ATTEMPT                                                                                                                                                              
        jsmn_parser p;
        jsmntok_t t[600]; /* We expect no more than 600 tokens */
        int r;

        jsmn_init(&p);
        r = jsmn_parse(&p, megillahBuf, strlen(megillahBuf), t, sizeof(t)/sizeof(t[0]));
        if (r < 0) {
          sprintf(content, "%sFailed to parse JSON: %d\n", content, r);
          //return 1;                                                                                                                                                                
        }


        /* Assume the top-level element is an object */
        if (r < 1 || t[0].type != JSMN_OBJECT) {
          sprintf(content, "%s<br>Object expected\n", content);
          //return 1;                                                                                                                                                                
        }

        int i;

        /* Loop over all keys of the root object */
        for (i = 1; i < r; i++) {
          if (jsoneq(megillahBuf, &t[i], "text") == 0) {
            /* We expect text to be an array of strings */
           int j;
           printf("- Text:\n<br>");
           if (t[i+1].type != JSMN_ARRAY) {
             continue; /* We expect groups to be an array of strings */
           }
           for (j = 0; j < t[i+1].size; j++) {
             jsmntok_t *g = &t[i+j+2];
             sprintf(content, "%s  <br><br> %.*s\n", content, g->end - g->start, megillahBuf + g->start);
           }
           i += t[i+1].size + 1;
          }
          else {
            sprintf(content, "%s<!--Unexpected key: %.*s\n-->", content, t[i].end-t[i].start, megillahBuf + t[i].start);
          }

        }

        // Trying to print the error to the screen                                                                                                                                   
        fprintf(stderr, "%s");

            
      }
    }
    else if (strncmp(megillah, "rut", 3) == 0) {
      // Making a wget call to Ruth and saving it to rut.txt.             
      int result = system("wget https://www.sefaria.org/api/texts/Ruth.1 -O rut.txt ");
      sprintf(content, "%s<p><i>Our algorithm has determined that appropriate seasonal learning for you is...</i></p><h1><u>Rut</u>: Chapter One</h1>", content); 
      // use rio to upload the result into a string                       
      int fd;
      fd = open("/home/sengel/csproj/netp/tiny/rut.txt", O_RDONLY, 0);
      if (fd < 0){
	sprintf(content, "%s<br>Open failed<br>%s\r\n", content);
      }
      
      // FIX TO NOT BE HARDCODED                                 
      int size = 26144;   
                  
      int status;
      status = rio_readn(fd, megillahBuf, size);   
      if (status == -1){                                          
	sprintf(content, "%sRead Error", content);        
      }
      else if (status == 0){             
	sprintf(content, "%sEOF zero returned from read", content);
      } else{
//sprintf(content, "%sSuccess. %d bytes transferred in read<br>\r\n", content, status);                                                                                      
        //Prints the API to the screen....                                                                                                                                           
        //sprintf(content, "%s%s", content, megillahBuf);                                                                                                                            

        // JSMN ATTEMPT                                                                                                                                                              
        jsmn_parser p;
        jsmntok_t t[600]; /* We expect no more than 600 tokens */
        int r;

        jsmn_init(&p);
        r = jsmn_parse(&p, megillahBuf, strlen(megillahBuf), t, sizeof(t)/sizeof(t[0]));
        if (r < 0) {
          sprintf(content, "%sFailed to parse JSON: %d\n", content, r);
          //return 1;                                                                                                                                                                
        }


        /* Assume the top-level element is an object */
        if (r < 1 || t[0].type != JSMN_OBJECT) {
          sprintf(content, "%s<br>Object expected\n", content);
          //return 1;                                                                                                                                                                
        }

        int i;

        /* Loop over all keys of the root object */
        for (i = 1; i < r; i++) {
          if (jsoneq(megillahBuf, &t[i], "text") == 0) {
            /* We expect text to be an array of strings */
           int j;
           printf("- Text:\n<br>");
           if (t[i+1].type != JSMN_ARRAY) {
             continue; /* We expect groups to be an array of strings */
           }
           for (j = 0; j < t[i+1].size; j++) {
             jsmntok_t *g = &t[i+j+2];
             sprintf(content, "%s  <br><br> %.*s\n", content, g->end - g->start, megillahBuf + g->start);
           }
           i += t[i+1].size + 1;
          }
          else {
            sprintf(content, "%s<!--Unexpected key: %.*s\n-->", content, t[i].end-t[i].start, megillahBuf + t[i].start);
          }

        }

        // Trying to print the error to the screen                                                                                                                                   
        fprintf(stderr, "%s");

                                     
	
      }
    }
    else{
      sprintf(content, "%sYou requested <b> %s </b><br>\r\n", content, megillah);
      sprintf(content, "%sERROR: %s is not a megillah<br>\r\n", content, megillah);
    }
    
    // Closing the div
    sprintf(content, "%s", content); // printng the entirety of content to the screen     
    sprintf(content, "%s<br><br><br><button class=\"button\"> <a href=\"http://ada.sterncs.net:6133/home.html\">Home</button></div></body></html>", content);
    
    /* Generate the HTTP response - which is just a bunch of print statements */
    
    // header of response - Going to be entirely different for our project. 
    printf("Connection: close\r\n");
    printf("Content-length: %d\r\n", (int)strlen(content));
    printf("Content-type: text/html\r\n\r\n");
    
    printf("%s", content);
    fflush(stdout);
    
    exit(0);
}
/* $end adder */
