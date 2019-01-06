/*
 * megillah.c as of 12/30
 * Having issues printing the substring API to screen
 * starting over in megillah.c substr-ying it, but 
 * wanted to save this copy for reference
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
      // Making a wget call to aicha
      // Saving it to aicha.txt.
      // TODO: Will need to generate random text file names so
      // multiple users can call without overwriting the system. 
      int result = system("wget https://www.sefaria.org/api/texts/Lamentations.1 -O aicha.txt ");
     
      // sprintf(content, "%sFile downloaded from API successfully<br>\r\n", content);
      // use rio to upload the strstr
      // Parse the result - strstr "text" ingest everything afterwards...
      // display the first perek to the user

      // use rio to upload the result into a string                                     
      // using ls -l know that esther.txt is 31127 bytes        
      int fd;
      fd = open("/home/sengel/csproj/netp/tiny/aicha.txt", O_RDONLY, 0);
      if (fd < 0){
        sprintf(content, "%s<br>Open failing<br>\r\n", content);    

        // Seeing the error message                                                        
        //char * message = strerror(errno);
	//sprintf(content, "%s<br>ERROR MESSAGE:<br>\r\n%s", content, message);
      }
      
      // FIX TO NOT BE HARDCODED                                
      int size = 30042;   
      
      //rio_readinitb(estherBuf, fd);                                         
      int status;
      status = rio_readn(fd, megillahBuf, size);   
      if (status == -1){               
	sprintf(content, "%sRead Error<br>\r\n", content);   
      }
      else if (status == 0){                            
	sprintf(content, "%sEOF zero returned from read<br>\r\n", content);
      }
      else{
        //working                                                            
        //sprintf(content, "%sSuccess. %d bytes transferred in read<br>\r\n", content, status);                
        // Prints the API to the screen....  
        // sprintf(content, "%s%s", content, megillahBuf);
	
	char firstChar[] = "text\":";
	char* p;
	
	// P will be a pointer to the first instance of
	// firstChar in megillahBuf
	p = strstr(megillahBuf, firstChar);
	
	//char * err_message = strerror(errno);
	
	//sprintf(content, "%s<br>ERROR MESSAGE:<br>\r\n%s", content, err_message);
	
	if (p){
	  // sprintf(content, "%sString found. First occurrence is at %d\r\n", content, &p);
	  int position = (p-megillahBuf) + 8; // The seven eliminates the 'text:"[' itself
	  int substringLength = strlen(p)-position;
	  
	  char subbuff[substringLength+1];
	  memcpy( subbuff, &megillahBuf[position], substringLength);
	  subbuff[substringLength] = '\0';
	  
	  char * endChar = "\"],";
	  // Now parse out the end part
	  char * firstHalf = subbuff;
	  char * d;
	  d = strstr(firstHalf, endChar);
	  
	  int endPos = (d-firstHalf);
	  
	  subbuff[endPos+1] = '\0';
	  
	  sprintf(content, "%s%s", content, subbuff);
	}
	else{
	  sprintf(content, "%sString not found.\r\n", content);
	}
	
      }
      // Parse the result  
      // display the first perek to the user 
    }
    else if (strncmp(megillah, "esther", 6) == 0) {
      // Making a wget call to esther
      // Saving it to esther.txt                            
      // TODO: Will need to generate random text file names so
      // multiple users can call without overwriting the system.                                                         
      int result = system("wget https://www.sefaria.org/api/texts/Esther.1 -O esther.txt ");
      //sprintf(content, "%sFile downloaded from API successfully<br>\r\n", content);
      sprintf(content, "%s<p><i>Our algorithm has determined that appropriate seasonal learning for you is...</i></p><h1><u>Esther</u>: Chapter One</h1>", content); 
      // use rio to upload the result into a string
      // using ls -l know that esther.txt is 31127 bytes
      
      int fd;
      fd = open("/home/sengel/csproj/netp/tiny/esther.txt", O_RDONLY, 0);
      
      if (fd < 0){
	sprintf(content, "%s<br>Open failing<br>\r\n", content);
	
	// Seeing the error message
	char * message = strerror(errno);
	sprintf(content, "%s<br>ERROR MESSAGE:<br>%s\r\n", content, message);
      }
      
      // FIX TO NOT BE HARDCODED
      int size = 31127;
      
      //rio_readinitb(estherBuf, fd);
      int status;
      status = rio_readn(fd, megillahBuf, size);
      
      //sprintf(content, "%sstatus = %d\r\n", content, status);
      if (status == -1){
	sprintf(content, "%sRead Error\r\n", content);
      }
      else if (status == 0){
	sprintf(content, "%sEOF zero returned from read\r\n", content);
      }
      else{
	// Not working
	//sprintf(content, "%sSuccess. %d bytes transferred in read<br>\r\n", content, status);
	// Not working....
	//sprintf(content, "%s\r\n%s", content, megillahBuf);
	
	char firstChar[] = "text\":";
	char * p;
	
	p = strstr(megillahBuf, firstChar);
	
	if (p){
	  int position = (p-megillahBuf)+8; // Plus eight to get rid of the 'text:"[' itself
	  int substringLength = strlen(p)-position;
	  
	  char subbuff[substringLength+1];
	  memcpy( subbuff, &megillahBuf[position], substringLength);
	  subbuff[substringLength] = '\0';
	  char * firstHalf = subbuff;
	  char * endChar = "\"],";
          // Now parse out the end part
	  char * d;
          d = strstr(firstHalf, endChar);                                                    
          int endPos = (d-firstHalf);
          subbuff[endPos+1] = '\0';
	  sprintf(content, "%s%s", content, subbuff);
	}
	
	else {
	  sprintf(content, "%sString not found.\r\n", content);
	}
      }
    }
    else if (strncmp(megillah, "kohelet", 7) == 0) {
      //char * message = strerror(errno);                                                                                                  
      //sprintf(content, "%s<br>ERROR MESSAGE:<br>\r\n%s", content, message);
      // Making a wget call to kohelet        
      // Saving it to kohelet.txt.                                        
      // TODO: Will need to generate random text file names so    
      // multiple users can call without overwriting the system.  
      int result = system("wget https://www.sefaria.org/api/texts/Ecclesiastes.1 -O kohelet.txt ");
      sprintf(content, "%s<p><i>Our algorithm has determined that appropriate seasonal learning for you is...</i></p><h1><u>Kohelet</u>: Chapter One</h1>", content); 
      // display the first perek to the user
      
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
      } else{

	// NEED TO RECOMMENT OUT
	// COMMENTING OUT NOW FOR DEBUGGING
        char firstChar[] = "\"text\": [";
        char * p;

	sprintf(content, "%s<h3>Printing megillahBuf</h3><br>%s", content, megillahBuf);
	
        p = strstr(megillahBuf, firstChar);
	
        if (p){
	int position = (p-megillahBuf) + 8; // Plus eight to get rid of the 'text:"[' itself                              
	int substringLength = strlen(p)-position;

	// BUG IS HINGED ON SOME ISSUE WITH MY
	// SUBBUFF -- BECAUSE NOT PRINTING AT ALL NOW
	// EVEN BEFORE THE WEIRD CODE MANIPULATION
	// TO EXTRACT THE END
	// SIZE SHOULDN'T BE AN ISSUE THOUGH
	char subbuff[substringLength+1];
	memcpy( subbuff, &megillahBuf[position], substringLength);
	subbuff[substringLength] = '\0';
	  
	char * firstHalf = subbuff;
	//sprintf(content, "%s<br><h3>First Half</h3><br>%s", content, firstHalf);
	//char * endChar = "\"],";
        //Now parse out the end part
      	//char * d;
	//d = strstr(firstHalf, endChar);
	  
	//int endPos = (d-firstHalf);
	//subbuff[endPos+1] = '\0';
	sprintf(content, "%sHello", content);
	//sprintf(content, "%s%s", content, subbuff);
        }
	
        else {
	  sprintf(content, "%sString not found.\r\n", content);
	}
      }
      // Parse the result                                                        
      // display the first perek to the user  
    }
    else if (strncmp(megillah, "shir", 4) == 0) {
      // Making a wget call to Song_of_Songs              
      // Saving it to shir.txt.                        
      // TODO: Will need to generate random text file names so                  
      // multiple users can call without overwriting the system.                      
      int result = system("wget https://www.sefaria.org/api/texts/Song_of_Songs.1 -O shir.txt ");
      
      //sprintf(content, "%sFile downloaded from API successfully<br>\r\n", content);
      sprintf(content, "%s<p><i>Our algorithm has determined that appropriate seasonal learning for you is...</i></p><h1><u>Shir HaShirim</u>: Chapter One</h1>", content); 
      // use rio to upload the result into a string
      // open file then use rio, slide 25 in lesson 17
      // Parse the result                                                     
      // display the first perek to the user
      // use rio to upload the result into a string                                                            
      int fd;
      fd = open("/home/sengel/csproj/netp/tiny/shir.txt", O_RDONLY, 0);
      if (fd < 0){
        sprintf(content, "%s<br>Open failing<br>\r\n", content);                    
	
	// Seeing the error message                                                        
	char * message = strerror(errno);
	sprintf(content, "%s<br>ERROR MESSAGE:%s<br>\r\n", content, message);
      }
      
      // FIX TO NOT BE HARDCODED                                                  
      int size = 17103;   
      
      //rio_readinitb(megillahBuf, fd);                                                   
      int status;
      status = rio_readn(fd, megillahBuf, size);   
      if (status == -1){                                                                  
	sprintf(content, "%sRead Error", content);                                
      }
      else if (status == 0){                                
	sprintf(content, "%sEOF zero returned from read", content);
      } else{
        //working                                                            
        // Not working                                                                                                     
        //sprintf(content, "%sSuccess. %d bytes transferred in read<br>\r\n", content, status);                                             
        // Not working....                                                                                                     
        //sprintf(content, "%s\r\n%s", content, megillahBuf);                                                                   
        char firstChar[] = "text\":";
        char * p;

        p = strstr(megillahBuf, firstChar);

        if (p){
          int position = (p-megillahBuf) + 8; // Plus eight to get rid of the 'text:"[' itself                                   
          int substringLength = strlen(p)-position;

          char subbuff[substringLength+1];
          memcpy( subbuff, &megillahBuf[position], substringLength);
          subbuff[substringLength] = '\0';
	  char * firstHalf = subbuff;
          char * endChar = "\"],";
          // Now parse out the end part                                                                                         
          char * d;
          d = strstr(firstHalf, endChar);
          int endPos = (d-firstHalf);
          subbuff[endPos+1] = '\0';
	  sprintf(content, "%s%s", content, subbuff);
        }

        else {
          sprintf(content, "%sString not found.\r\n", content);
        }          
      }
      // Parse the result                 
      // display the first perek to the user
    }
    else if (strncmp(megillah, "rut", 3) == 0) {
      // Making a wget call to Ruth      
      // Saving it to rut.txt.                       
      // TODO: Will need to generate random text file names so       
      // multiple users can call without overwriting the system.             
      int result = system("wget https://www.sefaria.org/api/texts/Ruth.1 -O rut.txt ");
      
      //sprintf(content, "%sFile downloaded from API successfully<br>\r\n", content);
      sprintf(content, "%s<p><i>Our algorithm has determined that appropriate seasonal learning for you is...</i></p><h1><u>Rut</u>: Chapter One</h1>", content); 
      // use rio to upload the result into a string
      // Parse the result                                  
      // display the first perek to the user
      // use rio to upload the result into a string                                                            
      int fd;
      fd = open("/home/sengel/csproj/netp/tiny/rut.txt", O_RDONLY, 0);
      if (fd < 0){
        sprintf(content, "%s<br>Open failing<br>\r\n", content);              
        // Seeing the error message                                       
        char * message = strerror(errno);
	sprintf(content, "%s<br>ERROR MESSAGE:<br>%s\r\n", content, message);
      }
      
      // FIX TO NOT BE HARDCODED                                 
      int size = 26144;   
      
      //rio_readinitb(megillahBuf, fd);                      
      int status;
      status = rio_readn(fd, megillahBuf, size);   
      if (status == -1){                                          
	sprintf(content, "%sRead Error", content);        
      }
      else if (status == 0){             
	sprintf(content, "%sEOF zero returned from read", content);
      } else{
        //working                                                            
        //sprintf(content, "%sSuccess. %d bytes transferred in read<br>\r\n", content, status);  
        // Not working....                                                                                              
        //sprintf(content, "%s\r\n%s", content, megillahBuf);                                                  

        char firstChar[] = "text\":";
        char * p;

        p = strstr(megillahBuf, firstChar);

        if (p){
          int position = (p-megillahBuf)+8; // Plus eight to get rid of the 'text:"[' itself                                                  
          int substringLength = strlen(p)-position;
          char subbuff[substringLength+1];
          memcpy( subbuff, &megillahBuf[position], substringLength);
          subbuff[substringLength] = '\0';
	  char * firstHalf = subbuff;
          char * endChar = "\"],";
          // Now parse out the end part                                                                                                     
          char * d;
          d = strstr(firstHalf, endChar);
          int endPos = (d-firstHalf);
          subbuff[endPos+1] = '\0';
	  sprintf(content, "%s%s", content, subbuff);
        }

        else {
          sprintf(content, "%sString not found.\r\n", content);
        }
      }
      // Parse the result               
      // display the first perek to the user
    }
    else{
      sprintf(content, "%sYou requested <b> %s </b><br>\r\n", content, megillah);
      sprintf(content, "%sERROR: %s is not a megillah<br>\r\n", content, megillah);
    }
    
    // Closing the div
    // body of response                                                                     
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
