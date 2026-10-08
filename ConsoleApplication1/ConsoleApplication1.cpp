/*
Среда: VS 2026
Выполнил: Поскряков Никита. Институт ispring ПС-21
24. Задан  текстовый  файл. Каждая строка содержит не более
255 символов. Создать  новый  файл,  в  котором  строки  будут
следовать в обратном порядке. Размер файла  не ограничивается.
Запрещается размещать файл целиком в  основной  памяти.  Файлы
размером порядка 10 Мгб должны обрабатываться не  более 1 сек.
(8).
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <string_view>
#include <algorithm>

using namespace std;

constexpr int MAX_LINES_IN_BUFFER = 100000;
constexpr long long CHUNK_SIZE = 1024;

struct Files {
    string InputFileName;
    string OutputFileName;
};

struct LinesBuffer {
    vector<string> lines;
    bool isEndOfFile = false;
    long long cursor = 0;

    void add(const string& line) {
        lines.push_back(line);
    }

    void clear() {
        lines.clear();
    }
};

Files InputFileNames() {
    Files files;
    cout << "Введите входной файл: ";
    cin >> files.InputFileName;
    cout << "Введите выходной файл: ";
    cin >> files.OutputFileName;
    return files;
}

void ReadToBuffer(ifstream& in, LinesBuffer& buffer) {
    buffer.lines.clear();
    if (buffer.cursor <= 0) {
        return;
    }

    long long bytesToRead = min(CHUNK_SIZE, buffer.cursor);
    long long readStart = buffer.cursor - bytesToRead;

    in.clear();
    in.seekg(readStart, ios::beg);

    string chunk(bytesToRead, '\0');
    in.read(&chunk[0], bytesToRead);

    size_t validStart = 0;

    if (readStart > 0) {
        size_t firstNewline = chunk.find('\n');
        if (firstNewline != string::npos) {
            validStart = firstNewline + 1;
        }
    }

    buffer.cursor = readStart + validStart;

    size_t end = bytesToRead;

    if (end > validStart && chunk[end - 1] == '\n') {
        end--;
        if (end > validStart && chunk[end - 1] == '\r') {
            end--;
        }
    }

    while (end > validStart) {
        size_t pos = chunk.rfind('\n', end - 1);

        if (pos == string::npos || pos < validStart) {
            string line = chunk.substr(validStart, end - validStart);
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }
            buffer.lines.push_back(line);
            break;
        }
        else {
            string line = chunk.substr(pos + 1, end - (pos + 1));
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }
            buffer.lines.push_back(line);

            end = pos;
            if (end > validStart && chunk[end - 1] == '\r') {
                end--;
            }
        }
    }
}

void WriteReverseBuffer(ofstream& out, const LinesBuffer& buffer) {
    for (const auto& line : buffer.lines) {
        out << line << "\r\n";
    }
}

bool ReverseLines(const Files& files) {
    ifstream in(files.InputFileName, ios::binary);
    if (!in.is_open()) return false;

    ofstream out(files.OutputFileName, ios::binary);
    if (!out.is_open()) return false;

    LinesBuffer buffer;
    in.seekg(0, ios::end);
    buffer.cursor = in.tellg();

    while (buffer.cursor > 0) {
        ReadToBuffer(in, buffer);
        WriteReverseBuffer(out, buffer);
    }
    return true;
}

int main() {
    Files files = InputFileNames();
    if (ReverseLines(files)) {
        cout << "Порядок строк успешно изменен." << endl;
        return 0;
    }
    else {
        cout << "Произошла ошибка при работе программы." << endl;
        return 1;
    }
}