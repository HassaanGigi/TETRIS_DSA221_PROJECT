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

class Hashmaps
{
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
class Tetrominos {
private:
    vector<vector<int>> Shapes;
    int x, y; // Coordinates of pieces
    Hashmaps tetromino;

public:
    // Define shapes
    vector<vector<int>> I_shape = { {1, 1, 1, 1} };
    vector<vector<int>> O_Shape = { {1, 1}, {1, 1} };
    vector<vector<int>> T_Shape = { {0, 1, 0}, {1, 1, 1} };
    vector<vector<int>> L_Shape = { {0, 0, 1}, {1, 1, 1} };
    vector<vector<int>> J_Shape = { {0, 0, 1}, {1, 1, 1} };
    vector<vector<int>> S_Shape = { {0, 1, 1}, {1, 1, 0} };
    vector<vector<int>> Z_Shape = { {1, 1, 0}, {0, 1, 1} };

    Tetrominos(const string& type) : x(4), y(0), tetromino(10) {
        tetromino.insert("I", I_shape);
        tetromino.insert("O", O_Shape);
        tetromino.insert("T", T_Shape);
        tetromino.insert("L", L_Shape);
        tetromino.insert("J", J_Shape);
        tetromino.insert("S", S_Shape);
        tetromino.insert("Z", Z_Shape);

        if (!tetromino.get(type, Shapes)) {
            throw invalid_argument("Invalid tetromino type");
        }
    }

void rotate(vector<vector<char>>& grid) {
    vector<vector<int>> newShape(Shapes[0].size(), vector<int>(Shapes.size()));
    for (size_t i = 0; i < Shapes[0].size(); i++) {
        for (size_t j = 0; j < Shapes.size(); j++) {
            newShape[j][Shapes.size() - 1 - i] = Shapes[i][j];
        }
    }
    
    // Try rotating first
    Tetrominos rotatedTetromino(*this); // Create a copy of the current tetromino
    rotatedTetromino.Shapes = newShape;

    // Check if the rotated shape is valid on the grid
    if (rotatedTetromino.canmoveto(grid, 0, 0)) {
        Shapes = newShape; // Only update if valid
    }
}



    vector<vector<int>> get_shape() {
        return Shapes;
    }

    void set_position(int new_x, int new_y) {
        x = new_x;
        y = new_y;
    }

    void move(int new_x, int new_y) {
        x += new_x;
        y += new_y;
    }

    bool canmoveto(const vector<vector<char>>& grid, int new_x, int new_y) {
        for (size_t i = 0; i < Shapes.size(); i++) {
            for (size_t j = 0; j < Shapes[i].size(); j++) {
                if (Shapes[i][j] == 1) {
                    int newPosX = x + j + new_x;
                    int newPosY = y + i + new_y;

                    if (newPosX < 0 || newPosX >= Grid_W || newPosY >= Grid_H) {
                        return false;
                    }

                    if (newPosY >= 0 && grid[newPosY][newPosX] == 'X') {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    void Place_Shape(vector<vector<char>>& grid) {
        for (size_t i = 0; i < Shapes.size(); i++) {
            for (size_t j = 0; j < Shapes[i].size(); j++) {
                if (Shapes[i][j] == 1) {
                    grid[y + i][x + j] = 'X';
                }
            }
        }
    }

    int GetX() const {
        return x;
    }

    int GetY() const {
        return y;
    }

    void move_left() {
        x--;
    }

    void move_right() {
        x++;
    }

    void move_down() {
        y++;
    }
};

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

class TetrisGrid {
private:
    vector<vector<char>> grid;

public:
    TetrisGrid() {
        // Initialize grid
        grid.resize(Grid_H, vector<char>(Grid_W, '.'));
    }

    void display( Tetrominos& current_Tetromino) {
        // Display the grid with the falling piece at the correct position
        for (int row = 0; row < Grid_H; ++row) {
            cout << "#";
            for (int col = 0; col < Grid_W; ++col) {
                // If a block is part of the tetromino, show it as 'X'
                bool is_block = false;
                for (int i = 0; i < current_Tetromino.get_shape().size(); ++i) {
                    for (int j = 0; j < current_Tetromino.get_shape()[i].size(); ++j) {
                        if (current_Tetromino.get_shape()[i][j] == 1) {
                            int posX = current_Tetromino.GetX() + j;
                            int posY = current_Tetromino.GetY() + i;
                            if (row == posY && col == posX) {
                                is_block = true;
                                break;
                            }
                        }
                    }
                    if (is_block) break;
                }
                cout << (is_block ? 'X' : grid[row][col]);
            }
            cout << "#" << endl;
        }
        cout << "############" << endl;  // Bottom border
    }

    vector<vector<char>>& GetGrid() {
        return grid;
    }

    void Clear_Lines() {
        for (int row = Grid_H - 1; row >= 0; --row) {
            bool line_complete = true;
            for (int col = 0; col < Grid_W; col++) {
                if (grid[row][col] == '.') {
                    line_complete = false;
                    break;
                }
            }
            if (line_complete) {
                for (int r = row; r > 0; --r) {
                    grid[r] = grid[r - 1];
                }
                grid[0] = vector<char>(Grid_W, '.');
                ++row;  // Checking same row again to ensure no errors.
            }
        }
    }
};

class Next_piece {
private:
    queue<string> Tetromino_next;
    const vector<string> Tetromino_types = { "I", "O", "S", "Z", "L", "T", "J" };

public:
    Next_piece() {
        // random piece generator
        srand(static_cast<unsigned int>(time(NULL)));
        for (int i = 0; i < 3; i++) {
            generate_next();
        }
    }

    void generate_next() {
        static vector<string> bag;
        if (bag.empty()) {
            bag = Tetromino_types;
            random_shuffle(bag.begin(), bag.end()); // randomizer
        }
        Tetromino_next.push(bag.back());
        bag.pop_back();
    }

    string Get_next() {
        if (Tetromino_next.empty()) { generate_next(); }
        string nextTetromino = Tetromino_next.front();
        Tetromino_next.pop();
        generate_next();
        return nextTetromino;
    }

    void display() {
        if (!Tetromino_next.empty()) {
            cout << "Next piece: " << Tetromino_next.front() << endl;
        } else {
            cout << "Next piece: Loading..." << endl;
        }
    }
};

#ifdef _WIN32
void ClearScreen() {
    COORD topleft = { 0,0 };
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;

    GetConsoleScreenBufferInfo(console, &screen);
    FillConsoleOutputCharacterA(console, ' ', screen.dwSize.X * screen.dwSize.Y, topleft, &written);
    FillConsoleOutputAttribute(console, screen.wAttributes, screen.dwSize.X * screen.dwSize.Y, topleft, &written);
    SetConsoleCursorPosition(console, topleft);
}
#else 
#include <cstdlib>
void ClearScreen() { system("clear"); }
#endif

enum GameState { RUNNING, GAME_OVER };
const int game_delay = 1000; // Reduced delay for smoother gameplay

long long Current_gametime_ms() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

void saveScore(const string& playerName, int score) {
    ofstream file("TetrisHighScores.txt", ios::app);
    if (file.is_open()) {
        time_t now = time(0);
        char datetime[100];
        strftime(datetime, sizeof(datetime), "%d-%m-%Y %H:%M:%S", localtime(&now));
        file << "Name: " << playerName << "\n";
        file << "Score: " << score << "\n";
        file << "Date/Time: " << datetime << "\n";
        file << "------------------\n";
        file.close();
    }
}

int main() {
    string playerName;
    cout << "Enter your name: ";
    getline(cin, playerName);

    int score = 0;
    int level = 0;
    int fall_interval = 1000;
    long long last_fall_time = Current_gametime_ms();
    GameState status = RUNNING;
    TetrisGrid game_grid;
    Next_piece nextPiece;
    Tetrominos current_Tetromino(nextPiece.Get_next());

    while (status == RUNNING) {
        if (_kbhit()) { // user controls
            int og_X = current_Tetromino.GetX();
            int og_Y = current_Tetromino.GetY();
            char input = _getch();
            switch (input) {
                case 'a':
                    current_Tetromino.move_left();
                    if (!current_Tetromino.canmoveto(game_grid.GetGrid(), 0, 0)) {
                        current_Tetromino.set_position(og_X, og_Y); // invalid move
                    }
                    break;

                case 'd':
                    current_Tetromino.move_right();
                    if (!current_Tetromino.canmoveto(game_grid.GetGrid(), 0, 0)) {
                        current_Tetromino.set_position(og_X, og_Y); // invalid move
                    }
                    break;
                case 's':
                    current_Tetromino.move_down();
                    if (!current_Tetromino.canmoveto(game_grid.GetGrid(), 0, 0)) {
                        current_Tetromino.set_position(og_X, og_Y); // invalid move
                    }
                    break;
                case 'w':
                    current_Tetromino.rotate(game_grid.GetGrid());
                    if (!current_Tetromino.canmoveto(game_grid.GetGrid(), 0, 0)) {
                        current_Tetromino.rotate(game_grid.GetGrid()); // rotate back
                        current_Tetromino.rotate(game_grid.GetGrid());
                        current_Tetromino.rotate(game_grid.GetGrid()); // rotate back to original
                    }
                    break;
            }
        }

        // making pieces fall without user input
        if (Current_gametime_ms() - last_fall_time >= fall_interval) {
        last_fall_time = Current_gametime_ms();
        if (current_Tetromino.canmoveto(game_grid.GetGrid(), 0, 1)) {
            current_Tetromino.move_down(); // Automatically falling pieces
        } else {
            current_Tetromino.Place_Shape(game_grid.GetGrid()); // Place the piece when it hits the bottom
            game_grid.Clear_Lines(); // To clear any completed lines.
            score += 100; // Increment score for placing a piece

            // Check if the new tetromino can be placed at the starting position
            current_Tetromino = Tetrominos(nextPiece.Get_next()); // Get the next tetromino
            if (!current_Tetromino.canmoveto(game_grid.GetGrid(), current_Tetromino.GetX(), current_Tetromino.GetY())) {
                status = GAME_OVER; // Game over only if the new piece cannot be placed at the starting position
            }
        }
    }


        // game interface
        ClearScreen();
        game_grid.display(current_Tetromino);
        nextPiece.display(); // display next piece
        level = 1 + score / 2000;
        fall_interval = max(100, 1000 - (level - 1) * 100); // max level 10 

        Sleep(game_delay);
    }

    cout << "Game Over! Final Score: " << score << endl;
    saveScore(playerName, score); // Save the score after the game ends
    return 0;
}