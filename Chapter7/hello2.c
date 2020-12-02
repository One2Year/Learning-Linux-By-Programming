/**
 * hello1.c
 * purpose      show how to use curses  function with a loop
 * outline      initialize, draw stuff, wrap up
*/

#include <stdio.h>
#include <curses.h>

main () {
    int i;
    initscr();      //turn on curses
                    //send requests
    clear();        //clear screen
    for (i = 0; i < LINES; ++i) {
        move(i, i+1);
        if (i % 2 == 1) standout();
        addstr("Hello world");
        if (i % 2 == 1) standend();
    }
    refresh();      //update the screen
    getch();        //wait for user input
    endwin();       //turn off curses
}