#include "GameState.h"

#include <cstdlib>

static constexpr int kMinLevel = 1;
static constexpr int kMaxLevel = 10;
static constexpr int kBaseDropSpeed = 40;
static constexpr int kDropSpeedStep = 3;
static constexpr int kMinDropSpeed = 5;

static int clampLevel(int level) {
    if (level < kMinLevel) return kMinLevel;
    if (level > kMaxLevel) return kMaxLevel;
    return level;
}

void InitializeGameState(GameState* state, int startingLevel) {
    if (!state) return;

    int clampedLevel = clampLevel(startingLevel);

    pthread_mutex_init(&state->stateMutex, nullptr);
    sem_init(&state->moveLeftSem, 0, 0);
    sem_init(&state->moveRightSem, 0, 0);
    sem_init(&state->rotateSem, 0, 0);
    sem_init(&state->softDropSem, 0, 0);
    sem_init(&state->gravitySem, 0, 0);
    sem_init(&state->lockSem, 0, 0);
    sem_init(&state->lineSem, 0, 0);
    sem_init(&state->scoreSem, 0, 0);
    sem_init(&state->levelSem, 0, 0);
    sem_init(&state->nextPieceSem, 0, 0);

    pthread_mutex_lock(&state->stateMutex);

    for (int r = 0; r < BoardRows; r++) {
        for (int c = 0; c < BoardCols; c++) {
            state->board[r][c] = 0;
        }
    }

    state->currentPiece = rand() % 7;
    state->nextPiece = rand() % 7;
    state->rotation = 0;
    state->posX = BoardCols / 2 - 2;
    state->posY = 0;
    state->score = 0;
    state->lines = 0;
    state->level = clampedLevel;
    state->baseLevel = clampedLevel;
    state->dropSpeed = kBaseDropSpeed - (clampedLevel * kDropSpeedStep);
    if (state->dropSpeed < kMinDropSpeed) state->dropSpeed = kMinDropSpeed;
    state->gameRunning = true;
    state->exitGame = false;
    state->activePieceReady = true;
    state->moveLeftRequest = false;
    state->moveRightRequest = false;
    state->rotateRequest = false;
    state->softDropRequest = false;
    state->gravityTick = false;
    state->lockRequest = false;
    state->lineClearPending = false;
    state->scorePending = false;
    state->levelPending = false;
    state->nextPiecePending = false;
    state->lastLinesCleared = 0;
    state->shutdown = false;

    pthread_mutex_unlock(&state->stateMutex);
}

void DestroyGameState(GameState* state) {
    if (!state) return;

    sem_destroy(&state->moveLeftSem);
    sem_destroy(&state->moveRightSem);
    sem_destroy(&state->rotateSem);
    sem_destroy(&state->softDropSem);
    sem_destroy(&state->gravitySem);
    sem_destroy(&state->lockSem);
    sem_destroy(&state->lineSem);
    sem_destroy(&state->scoreSem);
    sem_destroy(&state->levelSem);
    sem_destroy(&state->nextPieceSem);
    pthread_mutex_destroy(&state->stateMutex);
}
