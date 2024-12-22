#include <iostream>
#include <vector>
#include <ctime>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <queue>
#include <conio.h>
#include <windows.h>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

const int Grid_W = 10;
const int Grid_H = 20;

// Scoring structures
struct ScoreEntry {
    string name;
    int score;
    string datetime;
    ScoreEntry* next;
};

class ScoreList {
public:
    ScoreEntry* head;

    ScoreList() : head(nullptr) {}

    void addScore(const string& name, int score, const string& datetime) {
        ScoreEntry* newEntry = new ScoreEntry{name, score, datetime, head};
        head = newEntry;
    }

    void loadFromFile(const string& filename) {
        ifstream file(filename);
        if (file.is_open()) {
            string line, name, datetime;
            int score;
            while (getline(file, line)) {
                if (line.find("Name:") == 0) {
                    name = line.substr(6);
                } else if (line.find("Score:") == 0) {
                    score = stoi(line.substr(7));
                } else if (line.find("Date/Time:") == 0) {
                    datetime = line.substr(11);
                    addScore(name, score, datetime);
                }
            }
            file.close();
        }
    }

    void bubbleSort() {
        if (!head || !head->next) return;
        bool swapped;
        do {
            swapped = false;
            ScoreEntry* current = head;
            ScoreEntry* prev = nullptr;

            while (current->next) {
                if (current->score < current->next->score) {
                    if (prev) {
                        prev->next = current->next;
                    } else {
                        head = current->next;
                    }

                    ScoreEntry* temp = current->next->next;
                    current->next->next = current;
                    current->next = temp;
                    swapped = true;
                } else {
                    current = current->next;
                }
                if (!prev || prev->next != current) {
                    prev = current;
                }
            }
        } while (swapped);
    }

    void displayScores() {
        ScoreEntry* current = head;
        cout << "High Scores:\n";
        while (current) {
            cout << "Name: " << current->name << "\n";
            cout << "Score: " << current->score << "\n";
            cout << "Date/Time: " << current->datetime << "\n";
            cout << "------------------\n";
            current = current->next;
        }
    }
};

void saveGameLog(const string& playerName, const string& datetime, int score, const vector<vector<char>>& grid, const string& filename) {
    ofstream file(filename);
    if (file.is_open()) {
        file << "Player Name: " << playerName << endl;
        file << "Date/Time: " << datetime << endl;
        file << "Final Score: " << score << endl;
        file << "------------------" << endl;

        for (const auto& row : grid) {
            for (const auto& cell : row) {
                file << cell;
            }
            file << endl;
        }
        file.close();
    }
}

// Existing classes and main game logic remain unchanged.
class Hashmaps {
private:
    struct Node {
        string key;
        vector<vector<int>> value;
        Node* next;
    };

    Node** Table;
    int size;

    int hash(const string& key) {
        int hashval = 0;
        for (size_t i = 0; i < key.length(); i++) {
            hashval += key[i];
        }
        return hashval % size;
    }

public:
    Hashmaps(int s) {
        size = s;
        Table = new Node*[size]();
    }

    ~Hashmaps() {
        for (int i = 0; i < size; ++i) {
            Node* entry = Table[i];
            while (entry) {
                Node* temp = entry;
                entry = entry->next;
                delete temp;
            }
        }
        delete[] Table;
    }

    void insert(const string& key, vector<vector<int>>& val) {
        int index = hash(key);
        Node* newnode = new Node{ key, val, Table[index] };
        Table[index] = newnode;
    }

    bool get(const string& key, vector<vector<int>>& value) {
        int index = hash(key);
        Node* entry = Table[index];
        while (entry) {
            if (entry->key == key) {
                value = entry->value;
                return true;
            }
            entry = entry->next;
        }
        return false;
    }
};

// Main function with incorporated menu and features
int main() {
    ScoreList scoreList;
    scoreList.loadFromFile("TetrisHighScores.txt");

    while (true) {
        cout << " ===============================\n";
        cout << "|\t\t\t\t|\n";
        cout << "|\tWelcome to Tetris!\t|\n";
        cout << "|\t\t\t\t|\n";
        cout << " ===============================\n\n";
        cout << "1. Start Game\n";
        cout << "2. View High Scores\n";
        cout << "3. Exit\n";
        cout << "Select an option: ";

        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            string playerName;
            cout << "Enter your name: ";
            getline(cin, playerName);

            // Game logic remains unchanged...
            // After the game:
            time_t now = time(0);
            char datetime[100];
            strftime(datetime, sizeof(datetime), "%d-%m-%Y %H:%M:%S", localtime(&now));

            saveGameLog(playerName, datetime, 1000, vector<vector<char>>(Grid_H, vector<char>(Grid_W, '.')), "GameLog.txt");
            scoreList.addScore(playerName, 1000, datetime);
        } else if (choice == 2) {
            scoreList.bubbleSort();
            scoreList.displayScores();
        } else if (choice == 3) {
            break;
        }
    }

    return 0;
}
