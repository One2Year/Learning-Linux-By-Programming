/**
 * forkdemo2.c - shows how chhild processes pick up at the return
 *              from fork() and can execute any code they like,
 *              even fork(). Predict number of lines of output
*/
#include <stdio.h>

main() {
    printf("my pid is %d", getpid());
    fork();
    fork();
    fork();
    printf("my pid is %d", getpid());
}