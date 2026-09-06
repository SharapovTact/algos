#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
constexpr int MAX_LINES_IN_BUFFER = 10;

typedef struct {
	string InputFileName;
	string OutputFileName;
} Files;


Files InputFileNames() {
	Files files;
	cout << "Введите входной файл: ";
	cin >> files.InputFileName;
	cout << "Введите выходной файл: ";
	cin >> files.OutputFileName;
	return files;
}

int ReadLastLinesToBuffer(ifstream& in, vector<string>& buffer, const int bufferSize, int startPosition) {
	buffer.clear();
	int currentPosition = startPosition;
	int countLines = 0;
	string line;
	while (currentPosition > 0 && countLines < bufferSize) {
		in.clear();
		in.seekg(currentPosition, ios::beg);

		char ch;
		in.get(ch);

		if (ch == '\n') {
			in.seekg(currentPosition + 1, ios::beg);
			if (getline(in, line)) {
				buffer.push_back(line);
				countLines++;
				currentPosition--;
			}
		}
		currentPosition--;
	}
	if (currentPosition == 0 && countLines < bufferSize) {
		in.seekg(0, ios::beg);
		getline(in, line);
		buffer.push_back(line);
	}
	return currentPosition;
}

int CountCharsInFile(ifstream& in) {
	in.seekg(0, ios::end);
	int countChars = in.tellg();
	in.seekg(0, ios::beg);
	return countChars;
}

bool ReverseLines(Files files) {
	ifstream in(files.InputFileName);
	if (!in.is_open()) {
		return 0;
	}
	ofstream out(files.OutputFileName);
	if (!out.is_open()) {
		return 0;
	}
	string line;
	vector<string> linesBuffer;
	int cursorPosition = CountCharsInFile(in);
	bool firstLineWrited = false;
	while (cursorPosition > 0) {
		cursorPosition = ReadLastLinesToBuffer(in, linesBuffer, MAX_LINES_IN_BUFFER, cursorPosition);
		for (const auto& elem : linesBuffer) {
			if (!firstLineWrited) {
				out << elem;
				firstLineWrited = true;
			}
			else {
				out << endl << elem;
			}	
		}
	}
	in.close();
	out.close();
	return 1;
}

int main()
{
	Files files;
	files = InputFileNames();
	if (ReverseLines(files)) {
		cout << "порядок линий поменялся" << endl;
		return 0;
	}
	else {
		cout << "произошла ошибка при работе программы" << endl;
		return 1;
	}
}
