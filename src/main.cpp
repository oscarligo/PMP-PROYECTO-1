#include <ncurses.h>

int main() {
    initscr();              // Inicializar ncurses
    printw("Hola desde ncurses!");
    refresh();              // Mostrar en pantalla
    getch();                // Esperar una tecla
    endwin();               // Terminar ncurses
    return 0;
}
