#include <ncurses.h>
#include "Scores.h"
#include <fstream>
#include <vector>
#include <string>
#include <utility> 
#include <algorithm> 

using namespace std;

vector<pair<string, int>> ReadScores(const string &filename) {
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

void ShowScores() {
    clear();
    int row, col; getmaxyx(stdscr,row,col);

    auto scores = ReadScores("scores.txt");

    attron(A_BOLD);
    mvprintw(1, (col-10)/2, "PUNTAJES");
    attroff(A_BOLD);

    int y = 3;
    if (scores.empty()) {
        mvprintw(y, (col-30)/2, "No hay puntajes guardados todavía");
    } else {
        for (auto &p : scores) {
            mvprintw(y++, (col-30)/2, "%s : %d", p.first.c_str(), p.second);
        }
    }

    mvprintw(row-2, 2, "Presiona ESC para volver al menu");
    refresh();

    int ch;
    while ((ch = getch()) != 27) {
        // espera hasta presionar ESC
    }
}

void SaveScore(const string &name, int score) {
    ofstream file("scores.txt", ios::app);
    if (!file.is_open()) return;
    file << name << " " << score << "\n";
    file.close();
}
