#include <ncurses.h>
#include "menu.h"

// #include "constants.h" // Uncomment if constants are used

int main() {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    showMainMenu();

    endwin();
    return 0;
}
