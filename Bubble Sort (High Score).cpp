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