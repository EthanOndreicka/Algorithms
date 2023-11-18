// I did "g++ version in mac terminal and all it said was Apple clang version 13.0.0 so I hope that helps"

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

struct Node {
    // each line
    std::string data;
    // less than
    Node* left;
    // greather than or equal to
    Node* right;
    Node(const std::string& val) : data(val), left(nullptr), right(nullptr) {}
};

Node* insertUtil(Node* node, const std::string& data, const std::string& path) {
    if (node == nullptr) {
        std::cout << "Path for " << data << ": " << path << std::endl;
        return new Node(data);
    }

    if (data < node->data)
        node->left = insertUtil(node->left, data, path + "L");
    else if (data > node->data)
        node->right = insertUtil(node->right, data, path + "R");

    return node;
}

// function to insert each node into the BST
void insert(Node*& root, const std::string& data) {
    root = insertUtil(root, data, "");
}

// searches for the line within the BST
Node* search(Node* root, const std::string& key, int& comparisons, std::string& path) {
    if (root == nullptr || root->data == key) {
        comparisons++;
        if (root && root->data == key)
            std::cout << "Path for " << key << ": " << path << std::endl;
        return root;
    }

    comparisons++;
    if (key < root->data) {
        path += "L";
        return search(root->left, key, comparisons, path);
    }

    path += "R";
    return search(root->right, key, comparisons, path);
}

// function to read the file and node-ify it
void readFileAndInsert(const std::string& filename, Node*& root) {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return;
    }

    const int maxLines = 666; // 666 items in txt
    std::string lines[maxLines]; // Create an array of strings to store the lines

    int lineCount = 0; // Counter to keep track of the number of lines read

    std::string line;
    while (lineCount < maxLines && std::getline(inputFile, line)) {
        // Changes each line to lowercase and removes whitespace
        std::transform(line.begin(), line.end(), line.begin(), ::tolower);
        line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());

        lines[lineCount] = line; // Store each line in the array
        lineCount++;
    }

    inputFile.close();

    for (int i = 0; i < lineCount; ++i) {
        insert(root, lines[i]);
    }
}

// function for reading in the new magic items file
void searchStringsInBST(const std::string& filename, Node* root) {
    std::ifstream searchFile(filename);
    if (!searchFile.is_open()) {
        std::cerr << "Failed to open the search file." << std::endl;
        return;
    }

    int totalComparisons = 0; // Counter to keep track of comparisons made
    int totalLines = 0; // Counter to keep track of the number of lines read

    std::string line;
    while (std::getline(searchFile, line)) {
        // Changes each line to lowercase and removes whitespace
        std::transform(line.begin(), line.end(), line.begin(), ::tolower);
        line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());

        int comparisons = 0;
        std::string path = "";
        search(root, line, comparisons, path);

        if (comparisons > 0) {
            std::cout << "Comparisons for " << line << ": " << comparisons << std::endl;
            std::cout << "-----" <<std::endl;
            totalComparisons += comparisons;
            totalLines++;
        }
    }

    searchFile.close();

    if (totalLines > 0) {
        // computes the average amount of comparisons
        double averageComparisons = static_cast<double>(totalComparisons) / totalLines;
        std::cout << "===============================" <<std::endl;
        std::cout << "Average number of comparisons: " << averageComparisons << std::endl;
    } else {
        std::cout << "No lines found in the search file." << std::endl;
    }
}

int main() {
    Node* root = nullptr;
    std::cout << "===============================" <<std::endl;
    readFileAndInsert("magic_items.txt", root);
    std::cout << "===============================" <<std::endl;
    searchStringsInBST("magicitems-find-in-bst.txt", root);
    std::cout << "===============================" <<std::endl;

    return 0;
}

