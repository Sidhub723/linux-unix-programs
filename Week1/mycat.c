/*Using unbuffered IO => done using userspace library functions
  Preferably compile using flags --Wall, --Werror and --Wextra
  To cat a file, use the redirect the file's FD using '<' from your shell
*/


#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SIZE 512

int main(int argc, char **argv) {
    char buf[SIZE];
    int n;
    
    /*argc and argv are casted simply, to prevent compiler warnings of
    unused variables lol */
    (void)argc;
    (void)argv;

    while((n = read(STDIN_FILENO,buf,SIZE)) > 0){
        if(write(STDOUT_FILENO,buf,n) != n) {
            //printing to stderr, since it is unable to read stdout
            fprintf(stderr,"ERROR! Unable to write: %s\n",strerror(errno));
            exit(EXIT_FAILURE);
        }
    }	
    if(n < 0) {
        fprintf(stderr,"ERROR!Unable to read: %s\n",strerror(errno));
        exit(EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);

}
