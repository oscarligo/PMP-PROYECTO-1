#ifndef GAME_H
#define GAME_H

#include "Constants.h"

void StartGame(int startingLevel);
int Rotate(int px, int py, int r);
bool DoesPieceFit(int tetromino, int rotation, int posX, int posY, int board[BoardRows][BoardCols]);
void LockPiece(int tetromino, int rotation, int posX, int posY, int board[BoardRows][BoardCols]);
int ClearLines(int board[BoardRows][BoardCols]);

#endif
