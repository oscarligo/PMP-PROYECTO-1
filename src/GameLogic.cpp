#include "Game.h"

#include "Constants.h"

const char Tetrominos[7][17] = {
    "..X...X...X...X.",
    ".XX..X...X......",
    ".X...X...XX.....",
    "..X..XX..X......",
    ".X...XX...X.....",
    ".....XX..XX.....",
    ".X...XX..X......"
};

int Rotate(int px, int py, int r) {
    switch (r % 4) {
        case 0: return py * 4 + px;
        case 1: return 12 + py - (px * 4);
        case 2: return 15 - (py * 4) - px;
        case 3: return 3 - py + (px * 4);
    }
    return 0;
}

bool DoesPieceFit(int tetromino, int rotation, int posX, int posY, int board[BoardRows][BoardCols]) {
    for (int px = 0; px < 4; px++) {
        for (int py = 0; py < 4; py++) {
            int pi = Rotate(px, py, rotation);
            if (Tetrominos[tetromino][pi] == 'X') {
                int fx = posX + px;
                int fy = posY + py;
                if (fx < 0 || fx >= BoardCols || fy >= BoardRows) return false;
                if (fy >= 0 && board[fy][fx] != 0) return false;
            }
        }
    }
    return true;
}

void LockPiece(int tetromino, int rotation, int posX, int posY, int board[BoardRows][BoardCols]) {
    for (int px = 0; px < 4; px++) {
        for (int py = 0; py < 4; py++) {
            int pi = Rotate(px, py, rotation);
            if (Tetrominos[tetromino][pi] == 'X') {
                int fx = posX + px;
                int fy = posY + py;
                if (fy >= 0 && fy < BoardRows && fx >= 0 && fx < BoardCols)
                    board[fy][fx] = tetromino + 1;
            }
        }
    }
}

int ClearLines(int board[BoardRows][BoardCols]) {
    int cleared = 0;
    for (int r = BoardRows - 1; r >= 0; r--) {
        bool full = true;
    for (int c = 0; c < BoardCols; c++) {
            if (board[r][c] == 0) { full = false; break; }
        }
        if (full) {
            for (int y = r; y > 0; y--) {
                for (int c = 0; c < BoardCols; c++)
                    board[y][c] = board[y - 1][c];
            }
            for (int c = 0; c < BoardCols; c++) board[0][c] = 0;
            cleared++;
            r++;
        }
    }
    return cleared;
}
