#include "Game.h"

#include <ncurses.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <cstring>
#include <string>

#include "GameRender.h"
#include "GameState.h"
#include "GameThreads.h"
#include "Scores.h"

void StartGame(int startingLevel) {
    GameState gameState;
    InitializeGameState(&gameState, startingLevel);

    pthread_t gravityThread, softDropThread, moveLeftThread, moveRightThread, rotateThread;
    pthread_t lockThread, lineThread, scoreThread, levelThread, nextPieceThread;

    pthread_create(&gravityThread, nullptr, RunGravityThread, &gameState);
    pthread_create(&softDropThread, nullptr, RunSoftDropThread, &gameState);
    pthread_create(&moveLeftThread, nullptr, RunMoveLeftThread, &gameState);
    pthread_create(&moveRightThread, nullptr, RunMoveRightThread, &gameState);
    pthread_create(&rotateThread, nullptr, RunRotateThread, &gameState);
    pthread_create(&lockThread, nullptr, RunLockThread, &gameState);
    pthread_create(&lineThread, nullptr, RunLineClearThread, &gameState);
    pthread_create(&scoreThread, nullptr, RunScoreThread, &gameState);
    pthread_create(&levelThread, nullptr, RunLevelThread, &gameState);
    pthread_create(&nextPieceThread, nullptr, RunNextPieceThread, &gameState);

    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);

    int frameCounter = 0;
    bool running = true;

    while (running) {
        int ch = getch();
        if (ch != ERR) {
            switch (ch) {
                case 27: { // ESC
                    pthread_mutex_lock(&gameState.stateMutex);
                    gameState.exitGame = true;
                    gameState.gameRunning = false;
                    pthread_mutex_unlock(&gameState.stateMutex);
                    running = false;
                    break;
                }
                case 'a':
                case 'A':
                case KEY_LEFT: {
                    bool post = false;
                    pthread_mutex_lock(&gameState.stateMutex);
                    if (!gameState.shutdown) {
                        gameState.moveLeftRequest = true;
                        post = true;
                    }
                    pthread_mutex_unlock(&gameState.stateMutex);
                    if (post) sem_post(&gameState.moveLeftSem);
                    break;
                }
                case 'd':
                case 'D':
                case KEY_RIGHT: {
                    bool post = false;
                    pthread_mutex_lock(&gameState.stateMutex);
                    if (!gameState.shutdown) {
                        gameState.moveRightRequest = true;
                        post = true;
                    }
                    pthread_mutex_unlock(&gameState.stateMutex);
                    if (post) sem_post(&gameState.moveRightSem);
                    break;
                }
                case 's':
                case 'S':
                case KEY_DOWN: {
                    bool post = false;
                    pthread_mutex_lock(&gameState.stateMutex);
                    if (!gameState.shutdown) {
                        gameState.softDropRequest = true;
                        post = true;
                    }
                    pthread_mutex_unlock(&gameState.stateMutex);
                    if (post) sem_post(&gameState.softDropSem);
                    break;
                }
                case 'w':
                case 'W':
                case KEY_UP: {
                    bool post = false;
                    pthread_mutex_lock(&gameState.stateMutex);
                    if (!gameState.shutdown) {
                        gameState.rotateRequest = true;
                        post = true;
                    }
                    pthread_mutex_unlock(&gameState.stateMutex);
                    if (post) sem_post(&gameState.rotateSem);
                    break;
                }
                default:
                    break;
            }
        }

        frameCounter++;
        bool triggerGravity = false;
        pthread_mutex_lock(&gameState.stateMutex);
        if (!gameState.gameRunning) {
            running = false;
        } else if (!gameState.shutdown && frameCounter >= gameState.dropSpeed) {
            frameCounter = 0;
            gameState.gravityTick = true;
            triggerGravity = true;
        }
        pthread_mutex_unlock(&gameState.stateMutex);

        if (triggerGravity) sem_post(&gameState.gravitySem);

    DrawBoard(&gameState);

        pthread_mutex_lock(&gameState.stateMutex);
        if (!gameState.gameRunning) {
            running = false;
        }
        pthread_mutex_unlock(&gameState.stateMutex);

        usleep(16000);
    }

    pthread_mutex_lock(&gameState.stateMutex);
    gameState.shutdown = true;
    gameState.gameRunning = false;
    pthread_mutex_unlock(&gameState.stateMutex);

    sem_post(&gameState.moveLeftSem);
    sem_post(&gameState.moveRightSem);
    sem_post(&gameState.rotateSem);
    sem_post(&gameState.softDropSem);
    sem_post(&gameState.gravitySem);
    sem_post(&gameState.lockSem);
    sem_post(&gameState.lineSem);
    sem_post(&gameState.scoreSem);
    sem_post(&gameState.levelSem);
    sem_post(&gameState.nextPieceSem);

    pthread_join(gravityThread, nullptr);
    pthread_join(softDropThread, nullptr);
    pthread_join(moveLeftThread, nullptr);
    pthread_join(moveRightThread, nullptr);
    pthread_join(rotateThread, nullptr);
    pthread_join(lockThread, nullptr);
    pthread_join(lineThread, nullptr);
    pthread_join(scoreThread, nullptr);
    pthread_join(levelThread, nullptr);
    pthread_join(nextPieceThread, nullptr);

    int finalScore = 0;
    bool exitRequested = false;
    pthread_mutex_lock(&gameState.stateMutex);
    finalScore = gameState.score;
    exitRequested = gameState.exitGame;
    pthread_mutex_unlock(&gameState.stateMutex);

    DestroyGameState(&gameState);

    if (!exitRequested) {
        clear();
        int h, w;
        getmaxyx(stdscr, h, w);
        mvprintw(h / 2 - 3, (w - 10) / 2, "GAME OVER!");
        mvprintw(h / 2 - 1, (w - 26) / 2, "Puntuación final: %d", finalScore);
        const char* prompt = "Ingresa tu nombre (max 16 chars): ";
        int px = (w - static_cast<int>(strlen(prompt))) / 2;
        mvprintw(h / 2 + 1, px, "%s", prompt);
        nodelay(stdscr, FALSE);
        echo();
        curs_set(1);
    char NameBuffer[32];
    memset(NameBuffer, 0, sizeof(NameBuffer));
    move(h / 2 + 1, px + static_cast<int>(strlen(prompt)));
    getnstr(NameBuffer, 16);
        noecho();
        curs_set(0);
    std::string name = NameBuffer;
        while (!name.empty() && (name.back() == ' ' || name.back() == '\n' || name.back() == '\r' || name.back() == '\t')) {
            name.pop_back();
        }
        if (name.empty()) name = "Anon";
    SaveScore(name, finalScore);
        mvprintw(h / 2 + 3, (w - 40) / 2, "Puntaje guardado para %s. Presiona una tecla...", name.c_str());
        refresh();
        getch();
    }

    nodelay(stdscr, FALSE);
}
