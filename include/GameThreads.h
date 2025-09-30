#ifndef GAME_THREADS_H
#define GAME_THREADS_H

#include "GameState.h"

void* RunGravityThread(void* arg);
void* RunSoftDropThread(void* arg);
void* RunMoveLeftThread(void* arg);
void* RunMoveRightThread(void* arg);
void* RunRotateThread(void* arg);
void* RunLockThread(void* arg);
void* RunLineClearThread(void* arg);
void* RunScoreThread(void* arg);
void* RunLevelThread(void* arg);
void* RunNextPieceThread(void* arg);

#endif
