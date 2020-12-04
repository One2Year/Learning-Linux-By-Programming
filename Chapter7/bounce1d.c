/**
 * bounce1d.c
 * purpose      animation with user contrilled speed and direction
 * note         the handler does the animation 
 *              the main program reads keyboard input
 * compile      cc bounce1d.c set_ticker.c -l curses -o bounce1d
 * Operation    'Q' quit
 *              ' ' change dir
 *              's' make it slower
 *              'f' make if faster
*/

#include <stdio.h>
#include <curses.h>
#include <sys/time.h>
#include <signal.h>
#include <string.h>
#include "set_ticker.h"

/* some global settings main and the handler use*/

#define MESSAGE "hello"
#define BLANK   "     "

int row;    //current row
int col;    //current column
int dir;    //where we are going

int main () {
    int delay;          //bigger->slower
    int ndelay;         //new delay
    int c;              //user input
    void move_msg(int); //handler for timer

    initscr();
    crmode();
    noecho();
    clear();

    row = 10;
    col = 0;
    dir = 1;
    delay = 200;    //200ms=0.2s

    move(row, col);
    addstr(MESSAGE);
    signal(SIGALRM, move_msg);
    set_ticker(delay);

    while(1) {
        ndelay = 0;
        c = getch();
        if (c == 'Q') break;
        if (c == ' ') dir = -dir;
        if (c == 'f' && delay > 2) ndelay = delay / 2;
        if (c == 's') ndelay = delay * 2;
        if (ndelay > 0) {
            set_ticker(delay = ndelay);
        }
    }
    endwin();
    return 0;
}

void move_msg(int signum) {
    signal(SIGALRM, move_msg);
    move(row, col);
    addstr(BLANK);
    col += dir;
    move(row, col);
    addstr(MESSAGE);
    refresh();

    //now handle borders
    if (dir == -1 && col < 0) dir = 1;
    else if (dir == 1 && col + strlen(MESSAGE) >= COLS) dir = -1;
}

