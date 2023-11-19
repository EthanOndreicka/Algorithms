// I did "g++ version in mac terminal and all it said was Apple clang version 13.0.0 so I hope that helps"

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <map>
#include <vector>
#include <sstream>
#include <cctype>

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

// vertex constructor
struct Vertex {
    int id;
    bool processed;
    std::vector<Vertex*> neighbors;

    Vertex(int _id) : id(_id), processed(false) {}
};

std::vector<Vertex*> vertices; // Vector to store vertices

// Function to add a vertex
void addVertex(int& vertexCount) {
    Vertex* newVertex = new Vertex(vertexCount);
    vertexCount++; // Increment the vertex count
    std::cout << "Added vertex with ID: " << newVertex->id << std::endl;
    vertices.push_back(newVertex); // Add the new vertex to the vector
}

// Function to add an edge between vertices
// Partially helped by Stack Overflow
void addEdge(int vertexId1, int vertexId2) {
    if (vertexId1 < 0 || vertexId1 >= vertices.size() || vertexId2 < 0 || vertexId2 >= vertices.size()) {
        std::cerr << "Invalid vertex IDs" << std::endl;
        return;
    }

    // Add vertexId2 to vertexId1's neighbors
    vertices[vertexId1]->neighbors.push_back(vertices[vertexId2]);

    // Add vertexId1 to vertexId2's neighbors
    vertices[vertexId2]->neighbors.push_back(vertices[vertexId1]);

    // Output confirming the addition of the edge
    std::cout << "Edge added between Vertex " << vertexId1 << " and Vertex " << vertexId2 << std::endl;
}

// Function to find two integers in a string... provided by ChatGPT
std::pair<int, int> findEdgeNumbers(const std::string& input) {
    int numberOne = 0;
    int numberTwo = 0;
    size_t i = 0;

    while (i < input.size() && !isdigit(input[i])) {
        i++;
    }

    while (i < input.size() && isdigit(input[i])) {
        numberOne = numberOne * 10 + (input[i] - '0');
        i++;
    }

    while (i < input.size() && !isdigit(input[i])) {
        i++;
    }

    while (i < input.size() && isdigit(input[i])) {
        numberTwo = numberTwo * 10 + (input[i] - '0');
        i++;
    }

    return std::make_pair(numberOne, numberTwo);
}

// function to read and parse lines in file
// parsing this file has given me the largest headache I've ever had
void parseLines(const std::string& filename, Node* root) {
    std::ifstream file(filename); // Open the file
    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) { // Read lines from the file
        if (line.empty()) {
            // do stuff
        } 
        else if (line.compare(0, 2, "--") == 0) {
            // do comment
            std::cout << "Comment line" << std::endl;
        } 
        else if (line.compare(0, 3, "new") == 0) {
            // do stuff
            std::cout << "new graph made" << std::endl;
        } 
        else if (line.compare(0, 10, "add vertex") == 0) {
            // do stuff
            std::cout << "vertex added" << std::endl;
            // Extract the number after "add vertex"... provided by ChatGPT
            std::string numberStr = line.substr(11);
            int vertexNumber = std::stoi(numberStr); // Convert string to integer
            std::cout << "Vertex added with number: " << vertexNumber << std::endl;

            addVertex(vertexNumber);   
        } 
        else if (line.compare(0, 8, "add edge") == 0) {
            // do stuff
            std::cout << "edge added" << std::endl;
            // Get the pair of numbers
            std::pair<int, int> edgeNum = findEdgeNumbers(line);

            int vertexId1 = edgeNum.first;
            int vertexId2 = edgeNum.second;

            std::cout << "Vertex IDs: " << vertexId1 << " and " << vertexId2 << std::endl;

            // Now you can use vertexId1 and vertexId2 as needed
            addEdge(vertexId1, vertexId2);
        } 
        else {
            // I hope this doesn't print
            std::cout << "Error" << std::endl;
        }
    }
    file.close(); // Close the file when done
}


int main() {
    Node* root = nullptr;
    std::cout << "===============================" << std::endl;
    readFileAndInsert("magic_items.txt", root);
    std::cout << "===============================" << std::endl;
    searchStringsInBST("magicitems-find-in-bst.txt", root);
    std::cout << "===============================" << std::endl;
    parseLines("graphs1.txt", root);
    return 0;
}

