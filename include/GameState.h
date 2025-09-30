#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <pthread.h>
#include <semaphore.h>
#include "Constants.h"

struct GameState {
    int board[BoardRows][BoardCols];
    int currentPiece;
    int nextPiece;
    int rotation;
    int posX;
    int posY;
    int score;
    int lines;
    int level;
    int baseLevel;
    int dropSpeed; // en frames
    bool gameRunning;
    bool exitGame;
    bool activePieceReady;
    bool moveLeftRequest;
    bool moveRightRequest;
    bool rotateRequest;
    bool softDropRequest;
    bool gravityTick;
    bool lockRequest;
    bool lineClearPending;
    bool scorePending;
    bool levelPending;
    bool nextPiecePending;
    int lastLinesCleared;
    bool shutdown;
    pthread_mutex_t stateMutex;
    sem_t moveLeftSem;
    sem_t moveRightSem;
    sem_t rotateSem;
    sem_t softDropSem;
    sem_t gravitySem;
    sem_t lockSem;
    sem_t lineSem;
    sem_t scoreSem;
    sem_t levelSem;
    sem_t nextPieceSem;
};

void InitializeGameState(GameState* state, int startingLevel);
void DestroyGameState(GameState* state);

#endif
