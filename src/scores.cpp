#include <ncurses.h>
#include "scores.h"
#include <fstream>
#include <vector>
#include <string>
#include <utility> 
#include <algorithm> 

using namespace std;

vector<pair<string, int>> readScores(const string &filename) {
    vector<pair<string,int>> scores;
    ifstream file(filename);

    if (!file.is_open()) {
        // Si no existe el archivo, simplemente devuelve vector vacío
        return scores;
    }

    string name;
    int score;
    while (file >> name >> score) {
        scores.push_back({name, score});
    }

    file.close();

    // Ordenar de mayor a menor
    sort(scores.begin(), scores.end(), [](const pair<string,int> &a, const pair<string,int> &b) {
        return a.second > b.second;
    });

    return scores;
}

void showScores() {
    clear();
    int row, col; getmaxyx(stdscr,row,col);

    auto scores = readScores("scores.txt");

    attron(A_BOLD);
    mvprintw(1, (col-10)/2, "PUNTAJES");
    attroff(A_BOLD);

    int y = 3;
    for (auto &p : scores) {
        mvprintw(y++, (col-20)/2, "%s : %d", p.first.c_str(), p.second);
    }

    mvprintw(row-2, 2, "Presiona ESC para volver al menu");
    refresh();

    int ch;
    while ((ch = getch()) != 27) {
        // espera hasta presionar ESC
    }
}
