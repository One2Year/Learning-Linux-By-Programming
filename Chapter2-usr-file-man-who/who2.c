/*
 * who1.c - read /etc/utmp and list infor therein
 *        - suppresses empty records
 *        - formates time nicely
*/

#include <stdio.h>
#include <stdlib.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

//#define SHOWHOST        //include remote machine on output
void showtime(long);
void show_info(struct utmp *);

int main () {
    struct utmp utbuf;     //read info into here
    int utmpfd;                     //read from this descriptor
    //int reclen = sizeof(utbuf);

    if ((utmpfd = open(UTMP_FILE, O_RDONLY)) == -1) {
        perror(UTMP_FILE);      //UTMP_FILE is in utmp.h
        exit(1);
    }

    while (read(utmpfd, &utbuf, sizeof(utbuf)) == sizeof(utbuf)) {
        show_info(&utbuf);
    }
    close(utmpfd);
    return 0;
}

/*
 * show info()
 * displays contents of the utmp struct in human readable form
 * display nothing if record has no user name
*/
void show_info(struct utmp *utbufp) {
    if (utbufp->ut_type != USER_PROCESS) return;
    printf("%-8.8s", utbufp->ut_name);
    printf(" ");
    printf("%-8.8s", utbufp->ut_line);
    printf(" ");
    showtime(utbufp->ut_time);
#ifdef SHOWHOST
    if (utbufp->ut_host[0] != '\0')
        printf("(%s)", utbufp->ut_host);
#endif
    printf("\n");
}

/*
 * displays time in a format fit for human consuption
 * uses ctime to build a string then picks parts out of it
 * Note: %12.12s prints a string 12 chars wide and LIMITS
 * it to 12chars.
*/
void showtime(long timeval) {
    char *cp;
    cp = ctime(&timeval);
    printf("%12.12s", cp + 4);
}

