/**
 * forkdemo1.c
 * shows how fork creates two processes, distingguishable
 * by the different return values from fork()
*/

#include <stdio.h>

main() {
    int ret_from_fork, mypid;

    mypid = getpid();
    printf("Before: my pid is %d\n", mypid);

    ret_from_fork = fork();
    sleep(1);
    printf("After: my pid is %d, fork() said %d\n", getpid(), ret_from_fork);
}