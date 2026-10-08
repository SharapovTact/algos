#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

struct Node {
    string name;
    bool isFile;
    bool toCopy; 
    vector<Node*> children;
};

Node* loadTreeFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Ошибка: не удалось открыть файл " << filename << "!\n";
        return nullptr;
    }

    Node* root = new Node{ "Корневой_узел", false, false, {} };
    vector<Node*> path;
    path.push_back(root);

    vector<int> indents;
    indents.push_back(-1);

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        int spaceCount = 0;
        while (spaceCount < line.length() && line[spaceCount] == ' ') {
            spaceCount++;
        }

        string name = line.substr(spaceCount);
        if (name.empty()) continue;

        bool isFile = (name.find('.') != string::npos);
        Node* newNode = new Node{ name, isFile, false, {} };

        while (indents.back() >= spaceCount) {
            path.pop_back();
            indents.pop_back();
        }

        path.back()->children.push_back(newNode);

        if (!isFile) {
            path.push_back(newNode);
            indents.push_back(spaceCount);
        }
    }

    file.close();
    return root;
}

void printTree(Node* node, int depth = 0) {
    if (!node) return;
    for (int i = 0; i < depth; ++i) {
        cout << "  ";
    }
    cout << (node->isFile ? "[-] " : "[+] ") << node->name << endl;
    for (Node* child : node->children) {
        printTree(child, depth + 1);
    }
}

bool isCppFile(const string& name) {
    if (name.length() >= 4) {
        return name.substr(name.length() - 4) == ".cpp";
    }
    return false;
}

Node* copyTargetPaths(Node* node) {
    if (!node) return nullptr;

    bool isTargetFolder = false;
    if (!node->isFile && !node->children.empty()) {
        isTargetFolder = true;
        for (Node* child : node->children) {
            if (!child->isFile || !isCppFile(child->name)) {
                isTargetFolder = false;
                break;
            }
        }
    }

    Node* newNode = new Node{ node->name, node->isFile, false, {} };

    if (isTargetFolder) {
        for (Node* child : node->children) {
            newNode->children.push_back(new Node{ child->name, true, false, {} });
        }
        return newNode;
    }
    else {
        if (!node->isFile) {
            for (Node* child : node->children) {
                if (!child->isFile) {
                    Node* childCopy = copyTargetPaths(child);
                    if (childCopy) {
                        newNode->children.push_back(childCopy);
                    }
                }
            }
        }
    }

    if (!newNode->children.empty()) {
        return newNode;
    }
    else {
        delete newNode;
        return nullptr;
    }
}

Node* processTree(Node* root) {
    if (!root) return nullptr;

    Node* newRoot = new Node{ "Новый_диск:", false, false, {} };

    for (Node* child : root->children) {
        if (!child->isFile) {
            Node* copiedBranch = copyTargetPaths(child);
            if (copiedBranch) {
                newRoot->children.push_back(copiedBranch);
            }
        }
        else if (isCppFile(child->name)) {
            newRoot->children.push_back(new Node{ child->name, true, false, {} });
        }
    }
    return newRoot;
}

void freeTree(Node* node) {
    if (!node) return;
    for (Node* child : node->children) {
        freeTree(child);
    }
    delete node;
}

int main() {

    Node* root = nullptr;
    Node* newDriveRoot = nullptr;
    int choice;

    do {
        cout << "\n=== Меню ===\n";
        cout << "1. Загрузить дерево из файла\n";
        cout << "2. Показать исходное дерево\n";
        cout << "3. Сформировать новое дерево (перенести папки с *.cpp)\n";
        cout << "4. Показать новое дерево\n";
        cout << "0. Выход\n";
        cout << "Ваш выбор: ";
        cin >> choice;

        switch (choice) {
        case 1:
            if (root) freeTree(root);
            root = loadTreeFromFile("tree.txt");
            if (root) cout << "Дерево успешно загружено в оперативную память!\n";
            break;
        case 2:
            if (root) printTree(root);
            else cout << "Ошибка: Сначала загрузите дерево!\n";
            break;
        case 3:
            if (root) {
                if (newDriveRoot) freeTree(newDriveRoot);
                newDriveRoot = processTree(root);
                cout << "Новое дерево успешно сформировано!\n";
            }
            else {
                cout << "Ошибка: Сначала загрузите дерево!\n";
            }
            break;
        case 4:
            if (newDriveRoot) printTree(newDriveRoot);
            else cout << "Ошибка: Сначала сформируйте новое дерево!\n";
            break;
        case 0:
            cout << "Завершение работы...\n";
            break;
        default:
            cout << "Неверный ввод, попробуйте снова.\n";
        }
    } while (choice != 0);

    freeTree(root);
    freeTree(newDriveRoot);

    return 0;
}