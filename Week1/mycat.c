/*
* Using unbuffered IO => done using userspace library functions
* Preferably compile using flags --Wall, --Werror and --Wextra
* To cat a file, use the redirect the file's FD using '<' from your shell
* clock_gettime() may not work in some scenarios. Use -lrt flags in case of gcc compiler.
* Exit using Ctrl + D or Ctrl + C
*/


#include <errno.h>
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <time.h>
#include <unistd.h>

#define SIZE 512
double fetch_time() {
    struct timespec current;

    if(clock_gettime(CLOCK_REALTIME, &current) == -1) {
        fprintf(stderr,"ERROR!Unable to access time: %s\n",strerror(errno));
        exit(EXIT_FAILURE);
    }
    return current.tv_sec + current.tv_nsec*1e-9;

}
int main(int argc, char **argv) {
    double start_time = fetch_time();
    clock_t cpu_start_time = clock(); 
    char buf[SIZE];
    int n;
    printf("Exit using Crtl + D or Ctrl + C");
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
    printf("Real(astronomical) time taken is %.6lf\n",fetch_time()-start_time);
    clock_t cpu_end_time = clock();
    printf("CPU clock time taken is %.9lu\n",((cpu_end_time - cpu_start_time)*1000)/CLOCKS_PER_SEC);
    return (EXIT_SUCCESS);

}
