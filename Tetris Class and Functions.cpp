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
};