/**
 * bounce_async.c
 * purpose  animation with user control, using O_ASYNC on fd
 * note     set_ticker() sends SIGALRM, handler does animation
 *          keyboard sends SIGIO, main only calls pause()
 * complile cc bounce_async.c -l curses -o bounce_async
*/

#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include "set_ticker.h"

#define MESSAGE     "hello"
#define BLANK       "     "

int row = 10;
int col = 10;
int dir = 1;
int delay = 200;
int done = 0;

main () {
    void on_alarm(int);         //handler for alarm
    void on_input(int);         //handler for keybd
    void enable_kbd_signals();

    initscr();
    crmode();
    noecho();
    clear();

    signal(SIGIO, on_input);    //install a handler
    enable_kbd_signals();       //turn on kbd signals
    signal(SIGALRM, on_alarm);  //install alarm handler
    set_ticker(delay);          //start ticking
    move(row, col);             //get into position
    addstr(MESSAGE);

    while(!done) {
        pause();
    }

    endwin();
}

void on_input(int signum) {
    int c = getch();
    if (c == 'Q' || c == EOF)
        done = 1;
    else if (c == ' ')
        dir = -dir;
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
void enable_kbd_signals() {
    int fd_flags;
    fcntl(0, __F_SETOWN, getpid());
    fd_flags = fcntl(0, F_GETFL);
    fcntl(0, F_SETFL, (fd_flags|O_ASYNC));

}

