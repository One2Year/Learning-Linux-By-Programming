/**
 * bounce_async.c
 * purpose  animation with user control, using aio_read() rtc
 * note     set_ticker() sends SIGALRM, handler does animation
 *          keyboard sends SIGIO, main only calls pause()
 * complile cc bounce_aio.c -lrt -l curses -o bounce_aio
*/

#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <string.h>
#include <aio.h>
#include "set_ticker.h"

#define MESSAGE     "hello"
#define BLANK       "     "

int row = 10;
int col = 10;
int dir = 1;
int delay = 200;
int done = 0;

struct aiocb kbcbuf;

main () {
    void on_alarm(int);         //handler for alarm
    void on_input(int);         //handler for keybd
    void setup_aio_buffer();

    initscr();
    crmode();
    noecho();
    clear();

    signal(SIGIO, on_input);    //install a handler
    setup_aio_buffer();
    aio_read(&kbcbuf);

    signal(SIGALRM, on_alarm);  //install alarm handler
    set_ticker(delay);          //start ticking

    mvaddstr(row, col, MESSAGE);             //get into position
    addstr(MESSAGE);

    while(!done) {
        pause();
    }

    endwin();
}

void on_input(int signum) {
    int c;
    char *cp = (char*)kbcbuf.aio_buf; //cast to char

    //check for errors
    if (aio_error(&kbcbuf) != 0)
        perror("reading failed");
    else 
        //get number of chars read
        if (aio_return(&kbcbuf) == 1) {
            c = *cp;
            if (c == 'Q' || c == EOF)
                done = 1;
            else if (c == ' ')
                dir = -dir;
        }
    aio_read(&kbcbuf);  
}

void on_alarm(int signum) {
    signal(SIGALRM, on_alarm);      //reset, just in case
    mvaddstr(row, col, BLANK);      //note mvaddstr()
    col += dir;
    mvaddstr(row, col, MESSAGE);
    refresh();

    //now handl borders
    if (dir == -1 && col <= 0)
        dir = 1;
    else if (dir == 1 && col + strlen(MESSAGE) >= COLS)
        dir = -1;
}

//install a handler, tell kernel who to notify on input, enable signals
void setup_aio_buffer() {
    static char input[1];

    kbcbuf.aio_fildes   = 0;
    kbcbuf.aio_buf      = input;
    kbcbuf.aio_nbytes   = 1;
    kbcbuf.aio_offset   = 0;

    kbcbuf.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
    kbcbuf.aio_sigevent.sigev_signo = SIGIO;
}