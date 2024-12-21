struct TreeNode {
    ScoreEntry* entry;
    TreeNode* left;
    TreeNode* right;

    TreeNode(ScoreEntry* e) : entry(e), left(nullptr), right(nullptr) {}
};

class ScoreBST {
public:
    TreeNode* root;

    ScoreBST() : root(nullptr) {}

    void insert(ScoreEntry* entry) {
        root = insertRec(root, entry);
    }

    void writeToFile(const string& filename) {
        ofstream file(filename);
        if (file.is_open()) {
            writeInOrder(root, file);
            file.close();
        }
    }

private:
    TreeNode* insertRec(TreeNode* node, ScoreEntry* entry) {
        if (!node) return new TreeNode(entry);
        if (entry->score > node->entry->score) {
            node->left = insertRec(node->left, entry);
        } else {
            node->right = insertRec(node->right, entry);
        }
        return node;
    }

    void writeInOrder(TreeNode* node, ofstream& file) {
        if (!node) return;
        writeInOrder(node->left, file);
        file << "Name: " << node->entry->name << endl;
        file << "Score: " << node->entry->score << endl;
        file << "Date/Time: " << node->entry->datetime << endl;
        file << "------------------" << endl;
        writeInOrder(node->right, file);
    }
};