#include <ncurses.h>

#include <cstdlib>
#include <ctime>
#include "Menu.h"

// #include "Constants.h" // Uncomment if constants are used

int main() {
    // Inicializar generador de números aleatorios
    srand(time(nullptr));
    
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);
    // Inicializar colores (al menos el par 2 usado en instrucciones)
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_WHITE, COLOR_BLACK);   // Texto por defecto
        init_pair(2, COLOR_YELLOW, COLOR_BLACK);  // Titulos/ayuda
        // Colores para piezas (3..9)
        init_pair(3, COLOR_CYAN, COLOR_BLACK);    
        init_pair(4, COLOR_BLUE, COLOR_BLACK);    
        init_pair(5, COLOR_WHITE, COLOR_BLACK);   
        init_pair(6, COLOR_YELLOW, COLOR_BLACK);  
        init_pair(7, COLOR_GREEN, COLOR_BLACK);   
        init_pair(8, COLOR_MAGENTA, COLOR_BLACK); 
        init_pair(9, COLOR_RED, COLOR_BLACK);     
    }

    ShowMainMenu();

    endwin();
    return 0;
}
