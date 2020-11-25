/*
    more02.c - version 0.2 of more
    read and print 24 lines then pause for a few special commands
    feature of version 0.2 : reads from /dev/tty for commands

    Defects:
    1. When enter "space" or "q", usr needs to press "enter" and the cmd will be printed in termial
*/

#include <stdio.h>
#include <stdlib.h>
#define PAGELEN 24
#define LINELEN 512

void do_more(FILE * );
int see_more();

int main (int ac, char * av[]) {
    FILE * fp;
    if (ac == 1) {
        do_more(stdin);
    } else {
        while (--ac)
        if ((fp = fopen(*++av, "r")) != NULL) {
            do_more(fp);
            fclose(fp);
        } else {
            exit(1);
        }
    }
    return 0;
}

/*
    read PAGELIEN lines, then call see_more() for further instructions
*/
void do_more(FILE * fp) {
    char line[LINELEN];
    int num_of_lines = 0;
    int see_more(FILE *), reply;
    FILE *fp_tty;
    fp_tty = fopen("/dev/tty", "r");        //NEW: cmd stream
    if (fp_tty == NULL) {                   //if open fails
        exit(1);                            //no use in running
    }
    while (fgets(line, LINELEN, fp)) {      //more input
        if (num_of_lines == PAGELEN) {      //full screen?
            reply = see_more(fp_tty);       //NEW: pass FILE
            if (reply == 0) {               //n: done
                break;
            }
            num_of_lines -= reply;          //reset count
        }
        if (fputs(line, stdout) == EOF) {   //show line
            exit(1);                        //or die
        }
        num_of_lines++;                     //count it
    }
}

/*
    print message, wait for response, return # of lines to advance
    q means no, space means yes, CR means one line
*/
int see_more(FILE *cmd) {               //NEW: accepts arg
    int c;
    printf("\033[7m more?\033[m");      //reverse on a vt100 ???   
    while ((c = getc(cmd)) != EOF) {    //get response
        if (c == 'q') return 0;         // q -> N
        if (c == ' ') return PAGELEN;   //' ' -> next page
        if (c == '\n') return 1;        //Enter key -> 1 line
    }
    return 0;
}

/*
    follow up:
    1. 
*/