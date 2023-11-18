// I did "g++ version in mac terminal and all it said was Apple clang version 13.0.0 so I hope that helps"

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <map>
#include <vector>
#include <sstream>

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

void readGraphFile(const std::string& filename, Node* root){
    std::ifstream graphsFile("graphs1.txt");
    if (!graphsFile.is_open()) {
        std::cerr << "Failed to open the graphs file." << std::endl;
        return 1;
    }

    std::vector<std::string> graphLines;
    std::string graphLine;
    while (std::getline(graphsFile, graphLine)) {
        // Transform and store each line in the vector
        std::transform(graphLine.begin(), graphLine.end(), graphLine.begin(), ::tolower);
        graphLine.erase(std::remove_if(graphLine.begin(), graphLine.end(), ::isspace), graphLine.end());
        graphLines.push_back(graphLine);
    }
    graphsFile.close();
}

class Vertex {
private:
    int id;
    bool processed;
    std::vector<Vertex*> neighbors;

public:
    // Constructor
    Vertex(int identifier) : id(identifier), processed(false) {}

    // Getter for ID
    int getId() const {
        return id;
    }

    // Getter for processed status
    bool isProcessed() const {
        return processed;
    }

    // Setter for processed status
    void setProcessed(bool status) {
        processed = status;
    }

    // Add neighbor
    void addNeighbor(Vertex* neighbor) {
        neighbors.push_back(neighbor);
    }

    // Get neighbors
    std::vector<Vertex*> getNeighbors() const {
        return neighbors;
    }
};

class Graph {
private:
    int numVertices;
    std::vector<Vertex> vertices;
    std::vector<std::vector<int>> adjacencyMatrix;
    std::vector<std::vector<Vertex*>> adjacencyList;

public:
    // Constructor
    Graph(int n) : numVertices(n), vertices(n), adjacencyMatrix(n, std::vector<int>(n, 0)), adjacencyList(n) {}

    // Add edge
    void addEdge(int from, int to) {
        adjacencyMatrix[from][to] = 1;
        vertices[from].addNeighbor(&vertices[to]);
        adjacencyList[from].push_back(&vertices[to]);
    }

    // Display adjacency matrix
    void displayAdjacencyMatrix() const {
        std::cout << "Adjacency Matrix:" << std::endl;
        for (const auto& row : adjacencyMatrix) {
            for (int val : row) {
                std::cout << val << " ";
            }
            std::cout << std::endl;
        }
    }

    // Display adjacency list
    void displayAdjacencyList() const {
        std::cout << "Adjacency List:" << std::endl;
        for (int i = 0; i < numVertices; ++i) {
            std::cout << "Vertex " << i << " -> ";
            for (const Vertex* v : adjacencyList[i]) {
                std::cout << v->getId() << " ";
            }
            std::cout << std::endl;
        }
    }
};


void processLines(const std::string& filename, Node* root) {
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
            // Do nothing because line is a comment
        } 
        else if (line.compare(0, 3, "new") == 0) {
            // do stuff
        } 
        else if (line.compare(0, 10, "add vertex") == 0) {
            // do stuff
            // Extract the ID from the "add vertex" line
            std::istringstream iss(line);
            std::string addVertexStr, idStr;
            iss >> addVertexStr >> idStr; // Extract "add" and "vertex" strings

            if (idStr.empty()) {
                std::cout << "Invalid format for adding a vertex: " << line << std::endl;
            } else {
                try {
                    int id = std::stoi(idStr); // Convert the ID string to an integer
                    // Create a Vertex object with the extracted ID
                    Vertex* newVertex = new Vertex(id);
                    // Now you can use the newVertex object as needed (add it to your graph, etc.)
                    // For example:
                    // graph.addNode(newVertex);
                } catch (const std::invalid_argument& e) {
                    std::cout << "Invalid ID for adding a vertex: " << idStr << std::endl;
                }
            }
        } 
        else if (line.compare(0, 8, "add edge") == 0) {
            // do stuff
            // Extract the IDs from the "add edge" line
            std::istringstream iss(line);
            std::string addEdgeStr, sourceStr, destinationStr;
            iss >> addEdgeStr >> sourceStr >> destinationStr;

            if (sourceStr.empty() || destinationStr.empty()) {
                std::cout << "Invalid format for adding an edge: " << line << std::endl;
            } else {
                try {
                    int sourceVertexID = std::stoi(sourceStr); // Convert the source ID string to an integer
                    int destinationVertexID = std::stoi(destinationStr); // Convert the destination ID string to an integer

                    // Add an edge between vertices in theGraph
                    Graph.addEdge(sourceVertexID, destinationVertexID);
                } catch (const std::invalid_argument& e) {
                    std::cout << "Invalid IDs for adding an edge: " << sourceStr << ", " << destinationStr << std::endl;
                }
            }
        } 
        else {
            // If this prints then something went wrong
            std::cout << "Unrecognized line: " << line << std::endl;
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

    return 0;
}

