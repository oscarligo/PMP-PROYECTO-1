#include "GameRender.h"

#include <ncurses.h>
#include <cstring>

#include "Constants.h"
#include "Game.h"

void DrawBoard(GameState* gameState) {
    int displayBoard[BoardRows][BoardCols];
    int currentPiece, rotation, posX, posY;
    int score, lines, level;
    bool activePiece;

    pthread_mutex_lock(&gameState->stateMutex);
    for (int r = 0; r < BoardRows; r++) {
        for (int c = 0; c < BoardCols; c++) {
            displayBoard[r][c] = gameState->board[r][c];
        }
    }
    currentPiece = gameState->currentPiece;
    rotation = gameState->rotation;
    posX = gameState->posX;
    posY = gameState->posY;
    score = gameState->score;
    lines = gameState->lines;
    level = gameState->level;
    activePiece = gameState->activePieceReady;
    pthread_mutex_unlock(&gameState->stateMutex);

    if (activePiece) {
        for (int px = 0; px < 4; px++) {
            for (int py = 0; py < 4; py++) {
                int pi = Rotate(px, py, rotation);
                if (Tetrominos[currentPiece][pi] == 'X') {
                    int fx = posX + px;
                    int fy = posY + py;
                    if (fx >= 0 && fx < BoardCols && fy >= 0 && fy < BoardRows) {
                        displayBoard[fy][fx] = currentPiece + 1; // 1..7
                    }
                }
            }
        }
    }

    clear();

    int h, w;
    getmaxyx(stdscr, h, w);

    int inner_w = BoardCols * CellWidth;
    int board_w = inner_w + 2;
    int board_h = BoardRows + 2;
    int ox = (w - board_w) / 2;
    int oy = 2;

    attron(A_BOLD);
    mvprintw(oy - 1, ox, "TABLERO DE JUEGO");
    attroff(A_BOLD);

    mvprintw(oy, ox, "+");
    for (int i = 0; i < inner_w; i++) addch('-');
    addch('+');

    for (int r = 0; r < BoardRows; r++) {
        mvprintw(oy + 1 + r, ox, "|");
    for (int c = 0; c < BoardCols; c++) {
            int cell = displayBoard[r][c];
            if (cell > 0) {
                int colorPair = 2 + cell;
                if (has_colors()) attron(COLOR_PAIR(colorPair) | A_BOLD);
                addstr("[ ]");
                if (has_colors()) attroff(COLOR_PAIR(colorPair) | A_BOLD);
            } else {
                addstr("   ");
            }
        }
        addch('|');
    }

    mvprintw(oy + board_h - 1, ox, "+");
    for (int i = 0; i < inner_w; i++) addch('-');
    addch('+');

    attron(A_BOLD);
    mvprintw(oy, ox + board_w + 4, "SCORE: %d", score);
    mvprintw(oy + 1, ox + board_w + 4, "LINEAS: %d", lines);
    mvprintw(oy + 2, ox + board_w + 4, "NIVEL: %d", level);
    attroff(A_BOLD);

    mvprintw(oy + 4, ox + board_w + 4, "Controles:");
    mvprintw(oy + 5, ox + board_w + 6, "A / <- : Izquierda");
    mvprintw(oy + 6, ox + board_w + 6, "D / -> : Derecha");
    mvprintw(oy + 7, ox + board_w + 6, "S / v  : Bajar");
    mvprintw(oy + 8, ox + board_w + 6, "W / ^  : Rotar");
    mvprintw(oy + 9, ox + board_w + 6, "ESC    : Menu");

    mvprintw(h - 2, 2, "Presione ESC para volver al menu");

    refresh();
}
