#include "GameThreads.h"

#include <semaphore.h>
#include <cstdlib>

#include "Constants.h"
#include "Game.h"

namespace {

void ProcessMoveLeft(GameState* state) {
    bool keepRunning = true;
    while (keepRunning) {
        sem_wait(&state->moveLeftSem);
        pthread_mutex_lock(&state->stateMutex);
        if (state->shutdown) {
            keepRunning = false;
        } else if (state->moveLeftRequest && state->activePieceReady) {
            int newX = state->posX - 1;
            if (DoesPieceFit(state->currentPiece, state->rotation, newX, state->posY, state->board)) {
                state->posX = newX;
            }
        }
        state->moveLeftRequest = false;
        pthread_mutex_unlock(&state->stateMutex);
    }
}

void ProcessMoveRight(GameState* state) {
    bool keepRunning = true;
    while (keepRunning) {
        sem_wait(&state->moveRightSem);
        pthread_mutex_lock(&state->stateMutex);
        if (state->shutdown) {
            keepRunning = false;
        } else if (state->moveRightRequest && state->activePieceReady) {
            int newX = state->posX + 1;
            if (DoesPieceFit(state->currentPiece, state->rotation, newX, state->posY, state->board)) {
                state->posX = newX;
            }
        }
        state->moveRightRequest = false;
        pthread_mutex_unlock(&state->stateMutex);
    }
}

void ProcessRotate(GameState* state) {
    bool keepRunning = true;
    while (keepRunning) {
        sem_wait(&state->rotateSem);
        pthread_mutex_lock(&state->stateMutex);
        if (state->shutdown) {
            keepRunning = false;
        } else if (state->rotateRequest && state->activePieceReady) {
            int newRotation = (state->rotation + 1) % 4;
            if (DoesPieceFit(state->currentPiece, newRotation, state->posX, state->posY, state->board)) {
                state->rotation = newRotation;
            }
        }
        state->rotateRequest = false;
        pthread_mutex_unlock(&state->stateMutex);
    }
}

void ProcessSoftDrop(GameState* state) {
    bool keepRunning = true;
    while (keepRunning) {
        sem_wait(&state->softDropSem);
        bool triggerLock = false;
        pthread_mutex_lock(&state->stateMutex);
        if (state->shutdown) {
            keepRunning = false;
        } else if (state->softDropRequest && state->activePieceReady) {
            int newY = state->posY + 1;
            if (DoesPieceFit(state->currentPiece, state->rotation, state->posX, newY, state->board)) {
                state->posY = newY;
            } else if (!state->lockRequest) {
                state->lockRequest = true;
                triggerLock = true;
            }
        }
        state->softDropRequest = false;
        pthread_mutex_unlock(&state->stateMutex);
        if (triggerLock) sem_post(&state->lockSem);
    }
}

void ProcessGravity(GameState* state) {
    bool keepRunning = true;
    while (keepRunning) {
        sem_wait(&state->gravitySem);
        bool triggerLock = false;
        pthread_mutex_lock(&state->stateMutex);
        if (state->shutdown) {
            keepRunning = false;
        } else if (state->gravityTick && state->activePieceReady) {
            int newY = state->posY + 1;
            if (DoesPieceFit(state->currentPiece, state->rotation, state->posX, newY, state->board)) {
                state->posY = newY;
            } else if (!state->lockRequest) {
                state->lockRequest = true;
                triggerLock = true;
            }
        }
        state->gravityTick = false;
        pthread_mutex_unlock(&state->stateMutex);
        if (triggerLock) sem_post(&state->lockSem);
    }
}

void ProcessLock(GameState* state) {
    bool keepRunning = true;
    while (keepRunning) {
        sem_wait(&state->lockSem);
        bool triggerLine = false;
        bool triggerNext = false;
        pthread_mutex_lock(&state->stateMutex);
        if (state->shutdown) {
            keepRunning = false;
        } else if (state->lockRequest) {
            LockPiece(state->currentPiece, state->rotation, state->posX, state->posY, state->board);
            state->lockRequest = false;
            state->activePieceReady = false;
            state->lineClearPending = true;
            state->nextPiecePending = true;
            triggerLine = true;
            triggerNext = true;
        }
        pthread_mutex_unlock(&state->stateMutex);
        if (triggerLine) sem_post(&state->lineSem);
        if (triggerNext) sem_post(&state->nextPieceSem);
    }
}

void ProcessLineClear(GameState* state) {
    bool keepRunning = true;
    while (keepRunning) {
        sem_wait(&state->lineSem);
        bool triggerScore = false;
        pthread_mutex_lock(&state->stateMutex);
        if (state->shutdown) {
            keepRunning = false;
        } else if (state->lineClearPending) {
            int cleared = ClearLines(state->board);
            state->lastLinesCleared = cleared;
            state->lineClearPending = false;
            if (cleared > 0) {
                state->scorePending = true;
                triggerScore = true;
            } else {
                state->scorePending = false;
            }
        }
        pthread_mutex_unlock(&state->stateMutex);
        if (triggerScore) sem_post(&state->scoreSem);
    }
}

void ProcessScore(GameState* state) {
    static const int ScoreTable[5] = {0, 100, 300, 500, 800};
    bool keepRunning = true;
    while (keepRunning) {
        sem_wait(&state->scoreSem);
        bool triggerLevel = false;
        pthread_mutex_lock(&state->stateMutex);
        if (state->shutdown) {
            keepRunning = false;
        } else if (state->scorePending) {
            int cleared = state->lastLinesCleared;
            state->lines += cleared;
            state->score += ScoreTable[cleared] * state->level;
            state->scorePending = false;
            if (cleared > 0) {
                state->levelPending = true;
                triggerLevel = true;
            }
        }
        pthread_mutex_unlock(&state->stateMutex);
        if (triggerLevel) sem_post(&state->levelSem);
    }
}

void ProcessLevel(GameState* state) {
    bool keepRunning = true;
    while (keepRunning) {
        sem_wait(&state->levelSem);
        pthread_mutex_lock(&state->stateMutex);
        if (state->shutdown) {
            keepRunning = false;
        } else if (state->levelPending) {
            int newLevel = state->baseLevel + (state->lines / 10);
            state->level = newLevel;
            state->dropSpeed = 40 - (state->level * 3);
            if (state->dropSpeed < 5) state->dropSpeed = 5;
            state->levelPending = false;
        }
        pthread_mutex_unlock(&state->stateMutex);
    }
}

void ProcessNextPiece(GameState* state) {
    bool keepRunning = true;
    while (keepRunning) {
        sem_wait(&state->nextPieceSem);
        pthread_mutex_lock(&state->stateMutex);
        if (state->shutdown) {
            keepRunning = false;
        } else if (state->nextPiecePending) {
            state->currentPiece = state->nextPiece;
            state->nextPiece = rand() % 7;
            state->rotation = 0;
            state->posX = BoardCols / 2 - 2;
            state->posY = 0;
            state->nextPiecePending = false;
            if (DoesPieceFit(state->currentPiece, state->rotation, state->posX, state->posY, state->board)) {
                state->activePieceReady = true;
            } else {
                state->activePieceReady = false;
                state->gameRunning = false;
            }
        }
        pthread_mutex_unlock(&state->stateMutex);
    }
}

} // namespace

void* RunMoveLeftThread(void* arg) {
    ProcessMoveLeft(static_cast<GameState*>(arg));
    return nullptr;
}

void* RunMoveRightThread(void* arg) {
    ProcessMoveRight(static_cast<GameState*>(arg));
    return nullptr;
}

void* RunRotateThread(void* arg) {
    ProcessRotate(static_cast<GameState*>(arg));
    return nullptr;
}

void* RunSoftDropThread(void* arg) {
    ProcessSoftDrop(static_cast<GameState*>(arg));
    return nullptr;
}

void* RunGravityThread(void* arg) {
    ProcessGravity(static_cast<GameState*>(arg));
    return nullptr;
}

void* RunLockThread(void* arg) {
    ProcessLock(static_cast<GameState*>(arg));
    return nullptr;
}

void* RunLineClearThread(void* arg) {
    ProcessLineClear(static_cast<GameState*>(arg));
    return nullptr;
}

void* RunScoreThread(void* arg) {
    ProcessScore(static_cast<GameState*>(arg));
    return nullptr;
}

void* RunLevelThread(void* arg) {
    ProcessLevel(static_cast<GameState*>(arg));
    return nullptr;
}

void* RunNextPieceThread(void* arg) {
    ProcessNextPiece(static_cast<GameState*>(arg));
    return nullptr;
}
