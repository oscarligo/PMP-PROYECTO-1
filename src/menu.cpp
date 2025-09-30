#include <ncurses.h>
#include <cstring>
#include <string>
#include "Game.h"
#include "Scores.h"
#include "Instructions.h"
#include "Menu.h"


void ShowMainMenu() {

    const char *TitleBanner[] = {
        " ___________ ___________ ___________ __________  .___    _________ ",
        "\\__    ___/ \\_   _____/ \\__    ___/ \\______   \\ |   |  /   _____/ ",
        "  |    |     |    __)_    |    |     |       _/ |   |  \\_____  \\  ",
        "  |    |     |        \\   |    |     |    |   \\ |   |  /        \\ ",
        "  |____|    /_______  /   |____|     |____|_  / |___| /_______  / ",
        "                    \\/                      \\/                \\/  "
    };
    int TitleLineCount = 6;


    const int MinLevel = 1;
    const int MaxLevel = 10;
    int SelectedLevel = 1;

    const int OptionCount = 5; 
    int ChoiceIndex = 0;
    int InputKey = 0;

    // Asegurar modo bloqueante en menú
    nodelay(stdscr, FALSE);
    keypad(stdscr, TRUE);

    int IsRunning = 1;
    while (IsRunning) {
        clear();
        int ScreenRows, ScreenCols;
        getmaxyx(stdscr, ScreenRows, ScreenCols);

        for (int i = 0; i < TitleLineCount; i++) {
            mvprintw(ScreenRows/6 + i, (ScreenCols - strlen(TitleBanner[i]))/2, "%s", TitleBanner[i]);
        }


        for (int i = 0; i < OptionCount; i++) {
            std::string optionLabel;
            switch (i) {
                case 0: optionLabel = "Iniciar juego"; break;
                case 1: optionLabel = "Seleccionar nivel: " + std::to_string(SelectedLevel); break;
                case 2: optionLabel = "Ver puntajes"; break;
                case 3: optionLabel = "Instrucciones"; break;
                case 4: optionLabel = "Salir"; break;
                default: optionLabel = ""; break;
            }
            if (i == ChoiceIndex) attron(A_REVERSE); // resaltar opción activa
            mvprintw(ScreenRows/2 + i*2, (ScreenCols - static_cast<int>(optionLabel.size()))/2, "%s", optionLabel.c_str());
            if (i == ChoiceIndex) attroff(A_REVERSE);
        }

        // Mostrar mensajes de ayuda específicos
        if (ChoiceIndex == 1) {
            std::string hint = "Usa -> y <- o ENTER para ajustar el nivel (" +
                               std::to_string(MinLevel) + "-" + std::to_string(MaxLevel) + ")";
            mvprintw(ScreenRows/2 + OptionCount*2, (ScreenCols - static_cast<int>(hint.size()))/2, "%s", hint.c_str());
        } else if (ChoiceIndex == 4) {
            mvprintw(ScreenRows/2 + OptionCount*2, (ScreenCols - 30)/2, "Presiona ENTER para salir del menú");
        }

        refresh();

        InputKey = getch();
        switch (InputKey) {
            case KEY_UP:
                ChoiceIndex = (ChoiceIndex == 0) ? OptionCount - 1 : ChoiceIndex - 1;
                break;
            case KEY_DOWN:
                ChoiceIndex = (ChoiceIndex == OptionCount - 1) ? 0 : ChoiceIndex + 1;
                break;
            case KEY_LEFT:
            case 'a':
            case 'A':
                if (ChoiceIndex == 1 && SelectedLevel > MinLevel) {
                    SelectedLevel--;
                }
                break;
            case KEY_RIGHT:
            case 'd':
            case 'D':
                if (ChoiceIndex == 1 && SelectedLevel < MaxLevel) {
                    SelectedLevel++;
                }
                break;
            case '\n': 
                switch (ChoiceIndex) {
                    case 0:
                        StartGame(SelectedLevel);
                        break;
                    case 1:
                        // También permitir ajustar con ENTER
                        if (SelectedLevel < MaxLevel) {
                            SelectedLevel++;
                        } else {
                            SelectedLevel = MinLevel;
                        }
                        break;
                    case 2:
                        ShowScores();
                        break;
                    case 3:
                        ShowInstructions();
                        break;
                    case 4:
                        // Mensaje claro antes de salir
                        clear();
                        mvprintw(ScreenRows/2, (ScreenCols - 20)/2, "Saliendo del menú...");
                        refresh();
                        napms(1000);
                        return;
                }
                break;
            case 27: // ESC
                return;
        }
    }
}
