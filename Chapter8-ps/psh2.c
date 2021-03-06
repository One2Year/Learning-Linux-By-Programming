/**
 * prompting shell version2
 * 
 * Solves the 'one-shot' problem of version 1
 *      Uses execvp(), but fork()s first so that the shell waits around to perform another command
 * New Problem: shell catches signals. Run vi, press ^C
*/

#include <stdio.h>
#include <signal.h>

#define MAXARGS     20
#define ARGLEN      100

int main () {
    char *arglist[MAXARGS + 1];     //an array of ptrs
    int numargs;                    //index into array
    char argbuf[ARGLEN];            //read stuff here
    char *makestring();             //malloc etc
    numargs = 0;
    while (numargs < MAXARGS) {
        printf("Arg[%d]?", numargs);
        if (fgets(argbuf, ARGLEN, stdin) && *argbuf != '\n') {
            arglist[numargs++] = makestring(argbuf);
        } else {
            if (numargs > 0) {
                arglist[numargs] = NULL;
                execute(arglist);
                numargs = 0;
            }
        }
    }
    return 0;
}

// use execvp and wait to do it
int execute(char *arglist[]) {
    int pid, exitstatus;

    pid = fork();
    switch (pid)
    {
    case -1:
        perror("fork failed");
        exit(1);
    case 0:
        execvp(arglist[0], arglist);
        perror("execvp failed");
        exit(1);
    default:
        while(wait(&exitstatus) != pid)
            ;
        printf("child exited with status %d, %d\n", exitstatus>>8, exitstatus&0377);
    }
}

// trim off newline and create storage for the string
char *makestring(char *buf) {
    char *cp, *malloc();

    buf[strlen(buf) - 1] = '\0';    //trim newline
    cp = malloc(strlen(buf) + 1);   //get memory
    if (cp == NULL) {               //or die
        fprintf(stderr, "no memory\n");
        exit(1);
    }
    strcpy(cp, buf);    //copy chars
    return cp;          //return ptr
}