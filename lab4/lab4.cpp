/*22. Задан взвешенный ориентированный граф. С помощью алгоритма
Беллмана - Форда найти кратчайший путь между заданными вершинами.
Проверить наличие циклов отрицательной длины, достижимых из
начальной вершины. В случае обнаружения вывести один из них (10).*/
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;

struct Edge {
    int u, v;
    long long weight;
};

const long long INF = 1e18;

void BellermanFord(int n, int start, int finish, const vector<Edge>& edges) {
    vector<long long> dist(n + 1, INF);
    vector<int> parent(n + 1, -1);
    dist[start] = 0;
    int lastRelaxedNode = -1;

    for (int i = 0; i < n; i++) {
        lastRelaxedNode = -1;
        for (const auto& edge : edges) {
            if (dist[edge.u] < INF && dist[edge.v] > dist[edge.u] + edge.weight) {
                dist[edge.v] = max(-INF, dist[edge.u] + edge.weight);
                parent[edge.v] = edge.u;
                lastRelaxedNode = edge.v;
            }
        }
        if (lastRelaxedNode == -1) break;
    }

    if (lastRelaxedNode != -1) {
        cout << "Цикл отрицательной длины!В вершине: " << start << "!\n";

        int y = lastRelaxedNode;
        for (int i = 0; i < n; ++i) {
            y = parent[y];
        }

        vector<int> cycle;
        int curr = y;
        while (true) {
            cycle.push_back(curr);
            if (curr == y && cycle.size() > 1) break;
            curr = parent[curr];
        }
        reverse(cycle.begin(), cycle.end());

        cout << "Кишочки найденного цикла:\n";
        for (size_t i = 0; i < cycle.size(); i++) {
            cout << cycle[i] << (i + 1 < cycle.size() ? " -> " : "");
        }
        cout << endl;
        return;
    }

    cout << "Циклов отрицательной длины из" << start << ", не обнаружено.\n\n";

    cout << "Кратчайшее расстояния от вершины " << start << ":\n";
    for (int i = 1; i <= n; ++i) {
        cout << "До вершины " << i << ": ";
        if (dist[i] == INF) cout << "нет пути\n";
        else cout << dist[i] << endl;
    }

    cout << "\nКратчайший путь из " << start << " в " << finish << ": ";
    if (dist[finish] == INF) {
        cout << "пути не существует\n";
    }
    else {
        vector<int> path;
        for (int v = finish; v != -1; v = parent[v]) {
            path.push_back(v);
        }
        reverse(path.begin(), path.end());

        for (size_t i = 0; i < path.size(); ++i) {
            cout << path[i] << (i + 1 < path.size() ? " -> " : "");
        }
        cout << endl << "Длина пути: " << dist[finish] << endl;
    }
}

int main() {
    ifstream fin("input.txt");
    if (!fin.is_open()) {
        cout << "ERROR не удалось открыть файл input.txt\n";
        return 1;
    }

    int n, m, start, finish;
    fin >> n >> m >> start >> finish;
    if (start < 1 || start > n || finish < 1 || finish > n) {
        cout << "Ошибка номера начальной и конечной вершин должны быть в диапазоне >= 1" << n << "\n";
        return 1;
    }

    vector<Edge> edges(m);
    for (int i = 0; i < m; ++i) {
        fin >> edges[i].u >> edges[i].v >> edges[i].weight;
        if (edges[i].u < 1 || edges[i].u > n || edges[i].v < 1 || edges[i].v > n) {
            cout << "ошибка в ребре " << i + 1 << " неверный номер вершины\n";
            return 1;
        }
    }
    fin.close();

    BellermanFord(n, start, finish, edges);

    return 0;
}