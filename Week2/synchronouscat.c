/**********************************************************************************
 * WARNING - PROGRAM NOT WORKING AS INTENDED, DO NOT USE IT OR YOU MAY LOSE DATA***
*********************************************************************************** */


/*
* A simple cat program, but this time its synchronous, and much slower.
* The time statistics are useful to notice the difference between the 
* default asynchronous operating and forced synchronous operation.
* After compiling, use it with the redirection operators '<' and '>'
* For example: 
* 	$./synchronouscat.o <inputfile >outputfile
*/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define BUFSIZE 2048

int main(int argc, char **argv)
{
    int n, flags;
    char buf[BUFSIZE];
    // Per usual casting argc and argc to prevent compiler warnings
    (void)argc;
    (void)argv;
	
    /*Since we are operating on stdin, we cannot call open(2) and turn on synchronous mode
    * Hence we use fcntl and get the existing flags with the F_GETFL arg */
    if((flags = fcntl(STDOUT_FILENO, F_GETFL, 0) < 0)) {
        fprintf(stderr,"ERROR!Unable to get file descriptor flags of stderr: %s\n",strerror(errno));
	exit(EXIT_FAILURE);
    }

    flags |= O_SYNC; 		//XORing the O_SYNC flag to turn synchronous mode on
    
    if(fcntl(STDOUT_FILENO,F_SETFL,flags)< 0) {
	    fprintf(stderr,"ERROR!Unable to set O_SYNC flag: %s\n",strerror(errno));
	    exit(EXIT_FAILURE);
    }
    
    while((n = read(STDIN_FILENO,buf, BUFSIZE) > 0)) {
	if(write(STDOUT_FILENO, buf, n) != n) {
		fprintf(stderr,"ERROR!Unable to write: %s\n",strerror(errno));
		exit(EXIT_FAILURE);	
		}
	}
	if(n<0) {
		fprintf(stderr,"ERROR!Unable to read: %s\n",strerror(errno));
		exit(EXIT_FAILURE);
	}
	return EXIT_SUCCESS;

    
}

