/* 23. На  плоскости  расположена  система  из  N  шестеренок,
которая  приводится  в  движение  вращением  шестеренки  1  по
часовой стрелке.  Сцепленные шестеренки могут вращаться только
в    противоположных    направлениях.   Требуется   определить
направление движения каждой шестеренки  либо  установить,  что
систему   заклинит.   Некоторые   шестеренки   могут  остаться
неподвижными (10).*/

// Выполнил: Поскряков Никита
// Институт ispring, ПС-21

#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

vector<vector<int>> InputEdgesFromFile(ifstream& fin, const int numOfCluthes, const int numOfGears) {
    vector<vector<int>> edges(numOfCluthes, vector<int>(2));

    for (int i = 0; i < numOfCluthes; i++) {
        fin >> edges[i][0] >> edges[i][1];
        if (edges[i][0] < 1 || edges[i][0] > numOfGears ||
            edges[i][1] < 1 || edges[i][1] > numOfGears) {
            return {};
        }
    }
    return edges;
}

vector<vector<int>> FillAdjectivites(const vector<vector<int>>& edges, const int numOfGears, const int numOfCluthes) {
    vector<vector<int>> adjective(numOfGears + 1);
    for (int i = 0; i < numOfCluthes; ++i) {
        int u = edges[i][0];
        int v = edges[i][1];
        adjective[u].push_back(v);
        adjective[v].push_back(u);
    }
    return adjective;
}

int main()
{
    string filename;
    cout << "Введите имя файла: ";
    cin >> filename;

    ifstream fin(filename);
    if (!fin.is_open()) {
        cout << "Ошибка: не удалось открыть файл " << filename << endl;
        return 1;
    }

    int numOfGears;
    int numOfCluthes;

    if (!(fin >> numOfGears >> numOfCluthes)) {
        cout << "Ошибка: файл пуст или данные некорректны." << endl;
        return 1;
    }

    if (numOfGears < 2) {
        cout << "Ошибка: должно быть от 2-х шестерёнок." << endl;
        return 1;
    }
    if (numOfCluthes < 0) {
        cout << "Ошибка: не может быть отрицательное кол-во связей!" << endl;
        return 1;
    }

    vector<vector<int>> edges = InputEdgesFromFile(fin, numOfCluthes, numOfGears);
    if (edges.empty() && numOfCluthes > 0) {
        cout << "Некорректный ввод связей в файле (выход за пределы)." << endl;
        return 1;
    }

    fin.close();

    vector<vector<int>> adjective = FillAdjectivites(edges, numOfGears, numOfCluthes);
    vector<int> state(numOfGears + 1, 0);
    vector<int> q;
    int head = 0;
    bool is_jammed = false;

    state[1] = 1;
    q.push_back(1);

    while (head < q.size()) {
        int current = q[head++];
        for (int i = 0; i < adjective[current].size(); i++) {
            int neighbor = adjective[current][i];

            if (state[neighbor] == 0) {
                state[neighbor] = -state[current];
                q.push_back(neighbor);
            }
            else if (state[neighbor] == state[current]) {
                is_jammed = true;
                break;
            }
        }
        if (is_jammed) break;
    }

    if (is_jammed) {
        cout << "Систему заклинит." << endl;
    }
    else {
        cout << "Направления вращения:" << endl;
        for (int i = 1; i <= numOfGears; ++i) {
            cout << "Шестеренка " << i << ": ";
            if (state[i] == 1) {
                cout << "По часовой";
            }
            else if (state[i] == -1) {
                cout << "Против часовой";
            }
            else {
                cout << "Неподвижна";
            }
            cout << endl;
        }
    }

    return 0;
}