/*
 * megillah.c - a CGI program which returns the first chapter
 * of the most seasonally relevant megillah using the 
 * Sefaria API and user input.
 * 
 * HTML and CSS stylistic features based on guidlines provided
 * by W3 Schools in their online modules which can be 
 * accessed here: https://www.w3schools.com/html/
 *
 * Stat() code based on model here: 
 * https://www.linuxquestions.org/questions/programming-9/how-to-get-size-of-file-in-c-183360/ 
 *
 * JSMN Code based on samples in the package documentation online. 
 * Main documentation for JSMN: https://github.com/zserge/jsmn
 * 
 * Further References:
 *
 * https://blog.manash.me/parsing-json-using-c-quick-introduction-to-jsmn-libra * ry-92d1e49559e8
 * 
 * https://github.com/zserge/jsmn/blob/master/example/simple.c
 * 
 * https://github.com/alisdair/jsmn-example/blob/master/twitter.c
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

  // megillahBuf stores the API download
  // based on the user's selection
  char megillahBuf[35000];

  // Content stores the content which will be
  // sent to the webpage
  char content[MAXLINE*10];

  // Megillah stores the QUERY_STRING which is the
  // url portion after the "?" telling us which
  // Megillah to query the Sefaria API for
  char * megillah;

  // Parsing the URL, and saving to megillah
  megillah = strncat(getenv("QUERY_STRING"), "\0", 1);

  // Printing the head of the HTML file with the comments,
  // CSS style features, and beginning of the HTML
  sprintf(content,"%s <!--\nSarah Engel\nComputer Systems\nSCW, YU Fall 2018\nProfessor Ari Shamash \n\nThis is a network programming project using C to extend the CMU TINY server to retrieve dynamic content from an API, and return it to the client. I extended their existing cgi-bin program to query the Sefaria API of Jewish Texts (link to the API is at the bottom of the homepage) and then parsed the returned JSON using JSMN for C (https://github.com/zserge/jsmn). Note, the unused keys in the JSON are emitted to the comments of the page for easy cross-reference if I ever choose to incorporate other data items stored in the JSON. Please disregard for now. -->\n\n<html> \n<style> \n\ndiv.main{ \n    color:white; \n    font-family: Helvetica, sans-serif;  \n    text-decoration: none;\n}\n\n.button {\n    background-color:lightblue; \n    border-color: white;\n    border-width: thick;\n    color: DodgerBlue;\n    padding: 15px 32px; \n    text-align: center; \n    text-decoration: none; \n    display: inline-block; \n    font-size: 16px; \n    margin: 4px 2px; \n    cursor: pointer;\n}\n div.top{\n    text-align: center;\n} \n.button:hover {\n    background-color: white; \n    color: #FFDF80;\n}\n</style>\n\n<div class = \"main\"><head><title>Megillot!</title></head>\n<body style=\"background-color:DodgerBlue;\">\n", content);

  // Setting up st struct to get the bytes of the
  // returned API call when saved to a text file.
  struct stat st;
  
  /* Based on the user's selected megillah, query the appropriate
     API, parse using JSMN, and display to screen */
  
  if (strncmp(megillah, "aicha", 5) == 0){

    sprintf(content, "%s<div class = \"top\">\n<p><i>Our algorithm has determined that appropriate seasonal learning for you is...</i></p>\n<h1><u>Aicha</u>: Chapter One</h1></div>\n\n\n<ol>", content);

    // Making a wget call to aicha and saving it to aicha.txt. 
    int result = system("wget https://www.sefaria.org/api/texts/Lamentations.1 -O aicha.txt ");

    if (result == -1){
      printf("Failed to access API");
    }

    // Opening API into string    
    int fd;
    fd = open("/home/sengel/csproj/netp/tiny/aicha.txt", O_RDONLY, 0);
    if (fd < 0){
      sprintf(content, "%s<br>Open failing<br>\r\n", content);
    }
    
    // Getting and saving the size of the returned
    // API call txt file.
    stat("/home/sengel/csproj/netp/tiny/aicha.txt", &st);
    int size = st.st_size;   

    // Attempting to read megillahBuf
    // If successful, will begin parsing
    int status;
    status = rio_readn(fd, megillahBuf, size);   
    if (status == -1){               
      sprintf(content, "%sRead Error<br>\r\n", content);   
    }
    else if (status == 0){                            
      sprintf(content, "%sEOF zero returned from read<br>\r\n", content);
    }
    else {                   

      // Read successful
      // Begin parsing using JSMN module

      // Setting up needed variables for JSMN
      jsmn_parser p;
      jsmntok_t t[600]; /* We expect no more than 600 tokens */
      int r;

      // Initializing the JSMN parser
      jsmn_init(&p);

      // Attempting to parse
      r = jsmn_parse(&p, megillahBuf, strlen(megillahBuf), t, sizeof(t)/sizeof(t[0]));

      // Checking for two possible error codes
      if (r < 0) {
	sprintf(content, "%sFailed to parse JSON: %d\n", content, r);
      }
      if (r < 1 || t[0].type != JSMN_OBJECT) {
	sprintf(content, "%s<br>Object expected\n", content);
      }

      
      int i;
      
      /* Loop over all keys of the root object */
      for (i = 1; i < r; i++) {

	// Looking for "text" portion of API
	if (jsoneq(megillahBuf, &t[i], "text") == 0) {
	  /* We expect text to be an array of strings */
	  int j;
	 
	  if (t[i+1].type != JSMN_ARRAY) {
	    continue; 
	  }

	  // Once you hit "text" print each string
	  // with numbered list HTML format (to label
	  // the verses with numbers) and <br> in between
	  for (j = 0; j < t[i+1].size; j++) {
	    jsmntok_t *g = &t[i+j+2];
	    sprintf(content, "%s<br><li> %.*s</li>\n", content, g->end - g->start, megillahBuf + g->start);
	  }
	  
	  i += t[i+1].size + 1;
	}

	// If it's not the desired "text" portion of the array
	// then print those portions of the API to the comments
	// of the HTML page for easy use later if desired
	else {
	  sprintf(content, "%s\n<!--Ununsed key: %.*s-->", content, t[i].end-t[i].start, megillahBuf + t[i].start);
	  
	}
	
      }

      // End of the numbered list
      sprintf(content, "%s\n</ol>\n", content);
    }
    
  }
  else if (strncmp(megillah, "esther", 6) == 0) {

    // Making a wget call to esther and saving it to esther.txt              
    int result = system("wget https://www.sefaria.org/api/texts/Esther.1 -O esther.txt ");

    if (result == -1){
      printf("Failed to access API");
    }

    // Printing the head of the HTML file with the specifics for Esther
    sprintf(content, "%s<div class=\"top\">\n<p><i>Our algorithm has determined that appropriate seasonal learning for you is...</i></p>\n<h1><u>Esther</u>: Chapter One</h1></div>\n\n\n<ol>", content);
    
    // Opening the returned API results
    int fd;
    fd = open("/home/sengel/csproj/netp/tiny/esther.txt", O_RDONLY, 0);
    
    if (fd < 0){
      sprintf(content, "%s<br>Open failing<br>\r\n", content);
    }
    
    // Getting and saving the size of the returned                           
    // API call txt file.                                                    
    stat("/home/sengel/csproj/netp/tiny/esther.txt", &st);
    int size = st.st_size;

    // Reading the API results into megillahBuf
    int status;
    status = rio_readn(fd, megillahBuf, size);

    // Checking for read error codes
    if (status == -1){
      sprintf(content, "%sRead Error\r\n", content);
    }
    else if (status == 0){
      sprintf(content, "%sEOF zero returned from read\r\n", content);
    }
    else {

      // Using JSMN module to parse the API

      // Setting up necessary variables for JSMN
      jsmn_parser p;
      jsmntok_t t[600]; /* We expect no more than 600 tokens */
      int r;

      // Intializing the parser
      jsmn_init(&p);

      // Attempting to parse
      r = jsmn_parse(&p, megillahBuf, strlen(megillahBuf), t, sizeof(t)/sizeof(t[0]));

      // Checking for error codes
      if (r < 0) {
	sprintf(content, "%sFailed to parse JSON: %d\n", content, r);                                        
      }
      if (r < 1 || t[0].type != JSMN_OBJECT) {
	sprintf(content, "%s<br>Object expected\n", content);               
      }
      
      int i;
      
      /* Loop over all keys of the root object */
      for (i = 1; i < r; i++) {

	// Looking specifically for the "text" portion
	if (jsoneq(megillahBuf, &t[i], "text") == 0) {

	  /* We expect text to be an array of strings */
	  int j;
	  if (t[i+1].type != JSMN_ARRAY) {
	    continue; 
	  }

	  // For each one of the strings contained in
	  // "text" print to screen with HTML tags to make
	  // it the next item in an ordered list and desired
	  // spacing
	  for (j = 0; j < t[i+1].size; j++) {
	    jsmntok_t *g = &t[i+j+2];
	    sprintf(content, "%s<br><li> %.*s</li>\n", content, g->end - g->start, megillahBuf + g->start);
	  }
	  
	  i += t[i+1].size + 1;
	}

	// For every other tag which is not "text"
	// print to the comments of the HTML for easy
	// reference upon a later date
	else {
	  sprintf(content, "%s\n<!--Ununsed key: %.*s-->", content, t[i].end-t[i].start, megillahBuf + t[i].start);
	}
	
      }

      // End the ordered list
      sprintf(content, "%s\n</ol>\n", content);
    }
  }
  else if (strncmp(megillah, "kohelet", 7) == 0) {

    // Making a wget call to kohelet and saving it to kohelet.txt.
    int result = system("wget https://www.sefaria.org/api/texts/Ecclesiastes.1 -O kohelet.txt ");

    // Checking for errors
    if (result == -1){
      printf("Failed to access API");
    } 

    // Printing the top of the file with titles etc
    sprintf(content, "%s<div class=\"top\">\n<p><i>Our algorithm has determined that appropriate seasonal learning for you is...</i></p>\n<h1><u>Kohelet</u>: Chapter One</h1></div>\n\n\n<ol>", content); 
    
    // Opening the downloaded kohelet.txt file
    int fd;
    fd = open("/home/sengel/csproj/netp/tiny/kohelet.txt", O_RDONLY, 0);

    // Checking for error codes
    if (fd < 0){
      sprintf(content, "%s<br>Open failing<br>\r\n", content);   
    }
    
    // Getting and saving the size of the returned                           
    // API call txt file.                                                    
    stat("/home/sengel/csproj/netp/tiny/kohelet.txt", &st);
    int size = st.st_size;

    // Attempting to read kohelet.txt saved into fd
    // into the string megillahBuf for parsing
    int status;
    status = rio_readn(fd, megillahBuf, size);

    // Checking for read error codes
    if (status == -1){                                      
      sprintf(content, "%sRead Error", content);                      
    }
    else if (status == 0){                                                 
      sprintf(content, "%sEOF zero returned from read", content);
    }
    else {

      // If read successful, begin parsing

      // Set up the variables needed by the JSMN
      // module for parsing
      jsmn_parser p;
      jsmntok_t t[600]; /* We expect no more than 600 tokens */
      int r;

      // Initializing parser
      jsmn_init(&p);

      // Attempting to parse
      r = jsmn_parse(&p, megillahBuf, strlen(megillahBuf), t, sizeof(t)/sizeof(t[0]));

      // Checking for parse errors
      if (r < 0) {
	sprintf(content, "%sFailed to parse JSON: %d\n", content, r);       
      }
      if (r < 1 || t[0].type != JSMN_OBJECT) {
	sprintf(content, "%s<br>Object expected\n", content);
      }
      
      int i;
      
      /* Loop over all keys of the root object */
      for (i = 1; i < r; i++) {

	// Looking for the tag "text" specifically
	// to just extract the verses
	if (jsoneq(megillahBuf, &t[i], "text") == 0) {

	  /* We expect text to be an array of strings */
	  int j;
	  if (t[i+1].type != JSMN_ARRAY) {
	    continue; 
	  }

	  // For each string in "text", print with HTML
	  // tags to assign a value in an ordered list
	  // and spacing. 
	  for (j = 0; j < t[i+1].size; j++) {
	    jsmntok_t *g = &t[i+j+2];
	    sprintf(content, "%s<br><li> %.*s</li>\n", content, g->end - g->start, megillahBuf + g->start);
	  }
	  
	  i += t[i+1].size + 1;

	}

	// For every tag that's not "text" print to the HTML
	// comments for easy reference later
	else {
	  sprintf(content, "%s\n<!--Ununsed key: %.*s-->", content, t[i].end-t[i].start, megillahBuf + t[i].start);
	}
	
      }

      // End the ordered list
      sprintf(content, "%s\n</ol>\n", content);
    }
  }
  else if (strncmp(megillah, "shir", 4) == 0) {

    // Making a wget call to Song_of_Songs and saving it to shir.txt. 
    int result = system("wget https://www.sefaria.org/api/texts/Song_of_Songs.1 -O shir.txt ");

    // Checking for system() fail
    if (result == -1){
      printf("Failed to access API");
    }

    // Printing top of the HTML content to buffer
    sprintf(content, "%s<div class =\"top\">\n<p><i>Our algorithm has determined that appropriate seasonal learning for you is...</i></p>\n<h1><u>Shir HaShirim</u>: Chapter One</h1></div>\n\n\n<ol>", content); 

    // Opening the downloaded text file containing our API
    int fd;
    fd = open("/home/sengel/csproj/netp/tiny/shir.txt", O_RDONLY, 0);
    if (fd < 0){
      sprintf(content, "%s<br>Open failing<br>\r\n", content);
    }
    
    // Getting and saving the size of the returned                           
    // API call txt file.                                                    
    stat("/home/sengel/csproj/netp/tiny/shir.txt", &st);
    int size = st.st_size;

    // Reading the returned API text file
    // and reading to megillahBuf for parsing
    int status;
    status = rio_readn(fd, megillahBuf, size);

    // Checking for read errors
    if (status == -1){                                                 
      sprintf(content, "%sRead Error", content);                             
    }
    else if (status == 0){                                
      sprintf(content, "%sEOF zero returned from read", content);
    }

    else {
      // Begin parsing

      // Setting up variables needed by the JSMN module
      jsmn_parser p;
      jsmntok_t t[600]; /* We expect no more than 600 tokens */
      int r;

      // Initializing the jsmn_parser
      jsmn_init(&p);

      // Attempting to parse
      r = jsmn_parse(&p, megillahBuf, strlen(megillahBuf), t, sizeof(t)/sizeof(t[0]));

      // Checking for parse error codes
      if (r < 0) {
	sprintf(content, "%sFailed to parse JSON: %d\n", content, r); 
      }
      if (r < 1 || t[0].type != JSMN_OBJECT) {
	sprintf(content, "%s<br>Object expected\n", content);    
      }
      
      int i;
      
      /* Loop over all keys of the root object */
      for (i = 1; i < r; i++) {

	// Looking specifically for the "text" key
	if (jsoneq(megillahBuf, &t[i], "text") == 0) {
	  
	  /* We expect text to be an array of strings */
	  int j;
	  
	  if (t[i+1].type != JSMN_ARRAY) {
	    continue; 
	  }

	  // For each string, print with
	  // desired HTML tags to make a numbered
	  // list and add spacing
	  for (j = 0; j < t[i+1].size; j++) {
	    jsmntok_t *g = &t[i+j+2];
	    sprintf(content, "%s<br><li>%.*s</li>\n", content, g->end - g->start, megillahBuf + g->start);
	  }
	  i += t[i+1].size + 1;
	}

	// Print all unused keys from the API to the comments
	// of the HTML for easy future reference
	else {
	  sprintf(content, "%s\n<!--Ununsed key: %.*s-->", content, t[i].end-t[i].start, megillahBuf + t[i].start);
	}
	
      }

      // End of the numbered list
      sprintf(content, "%s\n</ol>\n", content);
    }
    
  }
  else if (strncmp(megillah, "rut", 3) == 0) {

    // Making a wget call to Ruth and saving it to rut.txt.             
    int result = system("wget https://www.sefaria.org/api/texts/Ruth.1 -O rut.txt ");

    // Checking for a system() error
    if (result == -1){
      printf("Failed to access API");
    }

    // Printing the top of the HTML file with the title
    sprintf(content, "%s<div class=\"top\">\n<p><i>Our algorithm has determined that appropriate seasonal learning for you is...</i></p>\n<h1><u>Rut</u>: Chapter One</h1></div>\n\n\n<ol>", content);
    
    // Opening the downloaded API txt file                       
    int fd;
    fd = open("/home/sengel/csproj/netp/tiny/rut.txt", O_RDONLY, 0);
    if (fd < 0){
      sprintf(content, "%s<br>Open failed<br>\r\n", content);
    }
    
    // Getting and saving the size of the returned                           
    // API call txt file.                                                    
    stat("/home/sengel/csproj/netp/tiny/rut.txt", &st);
    int size = st.st_size;

    // Attempting to read the file
    int status;
    status = rio_readn(fd, megillahBuf, size);

    // Check for error codes on read
    if (status == -1){                                          
      sprintf(content, "%sRead Error", content);        
    }
    else if (status == 0){             
      sprintf(content, "%sEOF zero returned from read", content);
    }
    else{
      // If the read was successful, begin parse

      // Setting up variables needed for JSMN
      jsmn_parser p;
      jsmntok_t t[600]; /* We expect no more than 600 tokens */
      int r;

      // Initializing the jsmn_parser
      jsmn_init(&p);

      // Attempting to parse
      r = jsmn_parse(&p, megillahBuf, strlen(megillahBuf), t, sizeof(t)/sizeof(t[0]));

      // Checking for parse error codes
      if (r < 0) {
	sprintf(content, "%sFailed to parse JSON: %d\n", content, r);     
      }
      if (r < 1 || t[0].type != JSMN_OBJECT) {
	sprintf(content, "%s<br>Object expected\n", content);           
      }
      
      int i;
      
      /* Loop over all keys of the root object */
      for (i = 1; i < r; i++) {

	// Searching specifically for "text" key
	if (jsoneq(megillahBuf, &t[i], "text") == 0) {
	  
	  /* We expect text to be an array of strings */
	  int j;
	  if (t[i+1].type != JSMN_ARRAY) {
	    continue; /* We expect groups to be an array of strings */
	  }

	  // For each string stored in "text", print
	  // with HTML tags to add to an ordered list
	  // and spacing
	  for (j = 0; j < t[i+1].size; j++) {
	    jsmntok_t *g = &t[i+j+2];
	    sprintf(content, "%s<br><li> %.*s</li>\n", content, g->end - g->start, megillahBuf + g->start);
	  }
	  
	  i += t[i+1].size + 1;
	}

	// Print the rest of the API content to the HTML
	// comments to easy future reference
	else {
	  sprintf(content, "%s\n<!--Ununsed key: %.*s-->", content, t[i].end-t[i].start, megillahBuf + t[i].start);
	}
	
      }

      // End of the ordered list
      sprintf(content, "%s\n</ol>", content);
      
    }
  }

  // If the URL contains a request for something which is not
  // one of our five megillot, print an error page in the browser
  else{
    sprintf(content, "%sYou requested <b> %s </b><br>\r\n", content, megillah);
    sprintf(content, "%sERROR: %s is not a megillah<br>\r\n", content, megillah);
  }
  
  // Printing the bottom of the HTML page with a home button
  // Some divs, and CSS stylistic features which also
  // need to be closed
  sprintf(content, "%s<br><br><br>\n<!--Using div \"top\" here to center the button, apologies for the confusing name. Used otherwise only to center the text at the top of each page. -->\n<div class=\"top\">\n  <button class=\"button\"> <a href=\"http://ada.sterncs.net:6133/home.html\">Home</button>\n</div>\n</div><!-- End of the \"main\" div-->\n\n</body>\n</html>", content);

  // Printing the entirety of the content buffer to the screen
  sprintf(content, "%s", content);     
  
  /* Generate the HTTP response - which is just a bunch of print statements */
  
  // header of response. 
  printf("Connection: close\r\n");
  printf("Content-length: %d\r\n", (int)strlen(content));
  printf("Content-type: text/html\r\n\r\n");
  
  printf("%s", content);
  fflush(stdout);
  
  exit(0);
}
/* $end megillah */
