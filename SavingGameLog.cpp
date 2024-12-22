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