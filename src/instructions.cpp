#include <ncurses.h>
#include "instructions.h"

void showInstructions() {
    clear();
    mvprintw(0, 0, "Mostrando instrucciones...");
    refresh();
    getch();
}
