#include <ncurses.h>
#include "scores.h"

void showScores() {
    clear();
    mvprintw(0, 0, "Mostrando puntajes...");
    refresh();
    getch();
}
