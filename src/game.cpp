#include <ncurses.h>
#include "game.h"

void startGame() {
    clear();
    mvprintw(0, 0, "Iniciando el juego...");
    refresh();
    getch();
}
