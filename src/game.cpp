#include <ncurses.h>
#include "game.h"
#include "constants.h"


void startGame() {
    clear();
    int board[BOARD_ROWS][BOARD_COLS] = {0};

    int h,w;
    getmaxyx(stdscr,h,w);

    int board_w = BOARD_COLS * 2 + 2;
    int board_h = BOARD_ROWS + 2;
    int ox = (w - board_w) / 2;
    int oy = 2;

    // Título
    attron(A_BOLD);
    mvprintw(oy-1, ox, "TABLERO DE JUEGO");
    attroff(A_BOLD);

    // Borde superior
    mvprintw(oy, ox, "+");
    for(int i=0;i<board_w-2;++i) addch('-');
    addch('+');

    // Filas del tablero
    for(int r=0;r<BOARD_ROWS;++r) {
        mvprintw(oy + 1 + r, ox, "|");
        for(int c=0;c<BOARD_COLS;++c) {
            if(board[r][c]) {
                mvprintw(oy + 1 + r, ox + 1 + c*2, "[]");
            } else mvprintw(oy + 1 + r, ox + 1 + c*2, "  ");
        }
        mvprintw(oy + 1 + r, ox + board_w -1, "|");
    }

    // Borde inferior
    mvprintw(oy + board_h -1, ox, "+");
    for(int i=0;i<board_w-2;++i) addch('-');
    addch('+');

    // Información lateral
    attron(A_BOLD);
    mvprintw(oy, ox + board_w + 4, "SCORE: 0");
    mvprintw(oy+1, ox + board_w + 4, "LINEAS: 0");
    mvprintw(oy+2, ox + board_w + 4, "NIVEL: 1");
    attroff(A_BOLD);

    mvprintw(oy+4, ox + board_w + 4, "NEXT:");
    mvprintw(oy+6, ox + board_w + 6, "[][][][]");

    mvprintw(oy+10, ox + board_w + 4, "Controles:");
    mvprintw(oy+11, ox + board_w + 6, "A / <- : Izquierda");
    mvprintw(oy+12, ox + board_w + 6, "D / -> : Derecha");
    mvprintw(oy+13, ox + board_w + 6, "S      : Bajar");
    mvprintw(oy+14, ox + board_w + 6, "W      : Rotar");
    mvprintw(oy+15, ox + board_w + 6, "ESC    : Menu");


    mvprintw(h-2, 2, "Presione ESC para volver al menu");    

    refresh();

    
    int ch;
    while ((ch = getch()) != 27) {
    }
}
