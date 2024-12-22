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