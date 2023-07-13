/*
* A simple infinite loop shell. Not the safest idea but anyways
* Just like a useful shell, it cannot be terminated by the interrupt signal
* created by Ctril + C. Sending an EOF by using Ctril + D works tho
* All programs being executed are linked to $PATH
*/

#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <unistd.h>

char* getinput(char *buffer, size_t length)
{
    printf("dir:");
    char cwd[PATH_MAX];
    if(getcwd(cwd, sizeof(cwd)) != NULL ) {
        printf("%s:::",cwd);
    }
    else {
        fprintf(stderr,"ERROR! Unable to get current working directory: %s\n",strerror(errno));
    }
    return fgets(buffer,length,stdin);
}

void sig_int(int signo) 
{
    printf("\nHandling SIGNINT!");
    (void)fflush(stdout);
}

int main(int argc, char **argv)
{
    char buf[BUFSIZ];
    pid_t pid;
    int status;
    (void)argc;
    (void)argv;
    if(signal(SIGINT, sig_int) == SIG_ERR) {
        fprintf(stderr,"ERROR! Issues in handling the signal!: %s\n",strerror(errno));
    }

    while(getinput(buf,sizeof(buf))) {
        buf[strlen(buf) -1 ] = '\0';    //null terminating the buf, safety measure

        if((pid = fork()) == -1) {
            fprintf(stderr,"ERROR!Unable to fork a child process: %s\n",strerror(errno));
            continue;                   // trying to fork once again
        }
        else if(pid == 0) {
            execlp(buf,buf,(char *)0);
        }

        if((pid = waitpid(pid, &status, 0) < 0)) {
            fprintf(stderr,"ERROR! Waited for child process to end but failed:%s\n",strerror(errno));

        }
        
    }
    exit(EX_OK);
}