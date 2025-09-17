#include <ncurses.h>
#include <cstring>
#include "game.h"
#include "scores.h"
#include "instructions.h"
#include "menu.h"


void showMainMenu() {
    const char *options[] = {
        "Iniciar juego",
        "Ver puntajes",
        "Instrucciones",
        "Salir"
    };

    int n_options = 4; 
    int choice = 0;
    int input;

    while (input != 3) {
        clear();
        int row, col;
        getmaxyx(stdscr, row, col);

    
        for (int i = 0; i < n_options; i++) {
            if (i == choice) attron(A_REVERSE); // resaltar opción activa
            mvprintw(row/2 + i*2, (col - strlen(options[i]))/2, "%s", options[i]);
            if (i == choice) attroff(A_REVERSE);
        }
        
        refresh();

        input = getch();
        switch (input) {
            case KEY_UP:
                choice = (choice == 0) ? n_options - 1 : choice - 1;
                break;
            case KEY_DOWN:
                choice = (choice == n_options - 1) ? 0 : choice + 1;
                break;
            case '\n': 
                switch (choice) {
                    case 0:
                        startGame();
                        break;
                    case 1:
                        showScores();
                        break;
                    case 2:
                        showInstructions();
                        break;
                    case 3:
                        return; // salir del menú
                }
                break;
        }
    }
}
