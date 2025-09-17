#include <ncurses.h>
#include "instructions.h"

void showInstructions() {
    clear();
    int h,w; getmaxyx(stdscr,h,w);

    attron(COLOR_PAIR(2) | A_BOLD);
    mvprintw(2, (w-25)/2, "INSTRUCCIONES DE JUEGO");
    attroff(COLOR_PAIR(2) | A_BOLD);

    mvprintw(4,4,"Objetivo:");
    mvprintw(5,6,"Completa lineas horizontales para eliminarlas y ganar puntos.");
    mvprintw(6,6,"El juego termina cuando las piezas llegan hasta arriba del tablero.");

    mvprintw(8,4,"Controles:");
    mvprintw(9,6,"A / <- : Mover izquierda");
    mvprintw(10,6,"D / -> : Mover derecha");
    mvprintw(11,6,"S      : Acelerar caida");
    mvprintw(12,6,"W      : Rotar pieza");
    mvprintw(13,6,"ESC    : Volver al menu");

    mvprintw(15,4,"Puntuacion:");
    mvprintw(16,6,"1 linea:   +100 puntos");
    mvprintw(17,6,"2 lineas:  +300 puntos");
    mvprintw(18,6,"3 lineas:  +500 puntos");
    mvprintw(19,6,"4 lineas:  +800 puntos (TETRIS!)");

    mvprintw(h-2, 2, "Presione ESC para volver al menu");    
    
    refresh();
    
    int ch;
    while ((ch = getch()) != 27) {
    }
}
