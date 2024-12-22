#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <conio.h>
#include <windows.h>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

#define COLUMNS 10
#define ROWS 20
#define GRID_SIZE 1

int gameCounter = 1; // Global counter for game files

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

const vector<vector<vector<int>>> TETROMINOES = {
    {{1, 1, 1, 1}},                     // I
    {{1, 1}, {1, 1}},                   // O
    {{0, 1, 0}, {1, 1, 1}},             // T
    {{0, 1, 1}, {1, 1, 0}},             // S
    {{1, 1, 0}, {0, 1, 1}},             // Z
    {{1, 0, 0}, {1, 1, 1}},             // J
    {{0, 0, 1}, {1, 1, 1}}              // L
};

void clearScreen() {
    COORD cursorPosition = {0, 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
}

class Tetris {
public:
    vector<vector<int>> grid;
    vector<vector<int>> currentPiece;
    vector<vector<int>> nextPiece;
    int pieceX, pieceY;
    int score;
    bool gameOver;

    Tetris() {
        grid = vector<vector<int>>(ROWS, vector<int>(COLUMNS, 0));
        currentPiece = getNewPiece();
        nextPiece = getNewPiece();
        pieceX = COLUMNS / 2 - 1;
        pieceY = 0;
        score = 0;
        gameOver = false;
    }

    vector<vector<int>> getNewPiece() {
        int pieceIndex = rand() % TETROMINOES.size();
        return TETROMINOES[pieceIndex];
    }

    void rotatePiece() {
        vector<vector<int>> rotatedPiece(currentPiece[0].size(), vector<int>(currentPiece.size()));
        for (int i = 0; i < currentPiece.size(); i++) {
            for (int j = 0; j < currentPiece[i].size(); j++) {
                rotatedPiece[j][currentPiece.size() - 1 - i] = currentPiece[i][j];
            }
        }
        currentPiece = rotatedPiece;
    }

    void movePiece(int dx, int dy) {
        pieceX += dx;
        pieceY += dy;
    }

    bool isCollision() {
        for (int i = 0; i < currentPiece.size(); i++) {
            for (int j = 0; j < currentPiece[i].size(); j++) {
                if (currentPiece[i][j]) {
                    int x = pieceX + j;
                    int y = pieceY + i;
                    if (x < 0 || x >= COLUMNS || y >= ROWS || (y >= 0 && grid[y][x])) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    void lockPiece() {
        for (int i = 0; i < currentPiece.size(); i++) {
            for (int j = 0; j < currentPiece[i].size(); j++) {
                if (currentPiece[i][j]) {
                    grid[pieceY + i][pieceX + j] = 1;
                }
            }
        }
        clearLines();
        currentPiece = nextPiece;
        nextPiece = getNewPiece();
        pieceX = COLUMNS / 2 - 1;
        pieceY = 0;
        if (isCollision()) {
            gameOver = true;
        }
    }

    void clearLines() {
        for (int i = 0; i < ROWS; i++) {
            if (all_of(grid[i].begin(), grid[i].end(), [](int x) { return x == 1; })) {
                grid.erase(grid.begin() + i);
                grid.insert(grid.begin(), vector<int>(COLUMNS, 0));
                score += 100;
            }
        }
    }

    void draw() {
        clearScreen();
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLUMNS; j++) {
                if (grid[i][j] || (i >= pieceY && i < pieceY + currentPiece.size() &&
                                   j >= pieceX && j < pieceX + currentPiece[0].size() &&
                                   currentPiece[i - pieceY][j - pieceX])) {
                    cout << "#";
                } else {
                    cout << ".";
                }
            }
            cout << endl;
        }
        cout << "Score: " << score << endl;
    }

    void saveGameLog(const string& playerName, const string& datetime, const string& filename) {
        ofstream file(filename);
        if (file.is_open()) {
            file << "Player Name: " << playerName << endl;
            file << "Date/Time: " << datetime << endl;
            file << "Final Score: " << score << endl;
            file << "------------------" << endl;

            for (int i = 0; i < ROWS; i++) {
                for (int j = 0; j < COLUMNS; j++) {
                    if (grid[i][j] || (i >= pieceY && i < pieceY + currentPiece.size() &&
                                       j >= pieceX && j < pieceX + currentPiece[0].size() &&
                                       currentPiece[i - pieceY][j - pieceX])) {
                        file << "#";
                    } else {
                        file << ".";
                    }
                }
                file << endl;
            }
            file.close();
        }
    }
};

int main() {
    srand(time(0));

    ScoreList scoreList;
    scoreList.loadFromFile("TetrisHighScores.txt");

    while (true) {
        // Main Menu
        cout << " ===============================\n";
        cout << "|\t\t\t\t|\n";
        cout << "|\t\t\t\t|\n";
        cout << "|\tWelcome to Tetris!\t|\n";
        cout << "|\t\t\t\t|\n";
        cout << "|\t\t\t\t|\n";
        cout << " ===============================\n\n" << endl;
        cout << "        1                       2                    3\n";                
        cout << " ==============       ==================== \t ======== \n";
        cout << "|              |     |                    |\t|        |\n";
        cout << "|  Start Game  |     |  View High Scores  |\t|  Exit  |\n";
        cout << "|              |     |                    |\t|        |\n";
        cout << " ==============       ==================== \t ======== \n\n";
		cout << "";

        cout << "Select an option: ";
        int choice;
        cin >> choice;
        cin.ignore(); // Ignore the leftover newline character from user input

        if (choice == 1) {
            // Start Game
            string playerName;
            cout << "Enter your name: ";
            getline(cin, playerName);

            Tetris game;
            int counter = 0;
            time_t now = time(0);
            char datetime[100];
            strftime(datetime, sizeof(datetime), "%d-%m-%Y %H:%M:%S", localtime(&now));

            stringstream ss;
            ss << "TetrisGame" << gameCounter++ << ".txt";
            string filename = ss.str();

            while (!game.gameOver) {
                if (_kbhit()) {
                    char key = _getch();
                    if (key == 'a') {
                        game.movePiece(-1, 0);
                        if (game.isCollision()) game.movePiece(1, 0);
                    } else if (key == 'd') {
                        game.movePiece(1, 0);
                        if (game.isCollision()) game.movePiece(-1, 0);
                    } else if (key == 's') {
                        game.movePiece(0, 1);
                        if (game.isCollision()) {
                            game.movePiece(0, -1);
                            game.lockPiece();
                        }
                    } else if (key == 'w') {
                        game.rotatePiece();
                        if (game.isCollision()) {
                            game.rotatePiece();
                            game.rotatePiece();
                            game.rotatePiece();
                        }
                    }
                }

                if (counter % 10 == 0) {
                    game.movePiece(0, 1);
                    if (game.isCollision()) {
                        game.movePiece(0, -1);
                        game.lockPiece();
                    }
                }

                game.draw();
                Sleep(50);
                counter++;
            }

            cout << "Game Over! Final Score: " << game.score << endl;

            // Save game log
            game.saveGameLog(playerName, datetime, filename);

            // Add new score
            scoreList.addScore(playerName, game.score, datetime);
            scoreList.bubbleSort();
        } 
        else if (choice == 2) {
            // View High Scores
            scoreList.displayScores();
        } 
        else if (choice == 3) {
            // Exit
            break;
        }
        else {
            cout << "Invalid option, please try again.\n";
        }
    }

    return 0;
}