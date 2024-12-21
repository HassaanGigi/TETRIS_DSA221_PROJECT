int main() {
    srand(time(0));

    ScoreList scoreList;
    scoreList.loadFromFile("TetrisHighScores.txt");

    while (true) {
        // Main Menu
        cout << "\t\t\t ===============================" << endl;
        cout << "\t\t\t|\t\t\t\t|\n";
        cout << "\t\t\t|\t\t\t\t|\n";
        cout << "\t\t\t|\tWelcome to Tetris!\t|\n";
        cout << "\t\t\t|\t\t\t\t|\n";
        cout << "\t\t\t|\t\t\t\t|\n";
        cout << "\t\t\t ===============================\n\n" << endl;
        cout << "\t\t\t        1                       2                    3\n";
        cout << "\t\t\t ==============       ==================== \t ======== \n";
        cout << "\t\t\t|              |     |                    |\t|        |\n";
        cout << "\t\t\t|  Start Game  |     |  View High Scores  |\t|  Exit  |\n";
        cout << "\t\t\t|              |     |                    |\t|        |\n";
        cout << "\t\t\t ==============       ==================== \t ======== \n\n";
		cout << "";
        cout << "\t\t\tSelect an option: ";
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

            // Get current date/time
            time_t now = time(0);
            char datetime[100];
            strftime(datetime, sizeof(datetime), "%d-%m-%Y %H:%M:%S", localtime(&now));

            // Add new score
            scoreList.addScore(playerName, game.score, datetime);
            scoreList.bubbleSort();

            // Store sorted scores in BST
            ScoreBST bst;
            for (ScoreEntry* entry = scoreList.head; entry; entry = entry->next) {
                bst.insert(entry);
            }

            // Write sorted scores to file
            bst.writeToFile("TetrisHighScores.txt");
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