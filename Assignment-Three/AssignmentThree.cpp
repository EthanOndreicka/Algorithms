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

    // Recursion!!!
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


// function to read the file and node-ify it
void readFileAndInsert(const std::string& filename) {
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

    Node* root = nullptr;
    for (int i = 0; i < lineCount; ++i) {
        insert(root, lines[i]);
    }
}

// Decided to take everything out of my main function unlike my past assignments
int main() {
    readFileAndInsert("magic_items.txt");
    return 0;
}
