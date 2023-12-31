#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <algorithm>
#include <cctype>
#include <cstddef>

// Constructs the node
class Node {
public:
    Node(const std::string& theItem) : item(theItem), next(nullptr) {}

    std::string item;
    Node* next;

    std::string getItem() {
        return item;
    }

    void setNext(Node* nextNode) {
        next = nextNode;
    }

    Node* getNext() {
        return next;
    }
};

// Define the size of the hash table
const int TABLE_SIZE = 250;

// Define a hash function
unsigned int hash(const std::string& key) {
    unsigned int hashValue = 0;
    for (size_t i = 0; i < key.length(); i++) {
        hashValue = 37 * hashValue + static_cast<unsigned int>(key[i]);
    }
    return hashValue % TABLE_SIZE;
}

// Class for items in the hash table
class HashItem {
public:
    std::string key;
    std::string value;
};

// Class for the hash table itself
class HashTable {
public:
    HashTable() {
        table.resize(TABLE_SIZE);
    }

    void insert(const std::string& key, const std::string& value) {
        unsigned int index = hash(key);
        Node* newNode = new Node(key);
        newNode->setNext(table[index]); // sets next pointer to current head
        table[index] = newNode;
    }

    bool get(const std::string& key, std::string& value, int& comparisons) {
        unsigned int index = hash(key);
        comparisons = 0;
        Node* current = table[index]; // starts at head
        while (current != nullptr) {
            comparisons++;
            if (current->getItem() == key) {
                value = current->getItem(); 
                return true;      // key is found
            }
            current = current->getNext();
        }
        return false;   // key not found
    }
    std::vector<Node*> table; // linked lists to store table
};


// Insertion Sort Function
void insertionSort(std::string lines[], int lineCount) {
    for (int i = 1; i < lineCount; i++) {
        std::string valueToInsert = lines[i];
        int j = i - 1;
        while (j >= 0 && lines[j] > valueToInsert) {
            lines[j + 1] = lines[j];
            j = j - 1;
        }
        lines[j + 1] = valueToInsert;
    }
}

// Linear Search Function
bool linearSearch(const std::string target, const std::string lines[], int lineCount, int& comparisons) {
    comparisons = 0; // counter for comparisons
    for (int i = 0; i < lineCount; i++) {
        comparisons++; // Increment the comparisons count for each iteration
        if (lines[i] == target) {
            return true; // Item found
        }
    }
    return false; // Item not found
}

// Binary Search Function
bool binarySearch(const std::string target, const std::string lines[], int lineCount, int& comparisons) {
    comparisons = 0; // comparisons counter
    int leftValue = 0;
    int rightValue = lineCount - 1;
    while (leftValue <= rightValue) {
        comparisons++; // count for each iteration
        int middleValue = leftValue + (rightValue - leftValue) / 2;
        if (lines[middleValue] == target) {
            return true; // Item found
        }
        if (lines[middleValue] < target) {
            leftValue = middleValue + 1;
        } else {
            rightValue = middleValue - 1;
        }
    }
    return false; // Item not found
}

// Random item chooser partially from ChatGPT
void chooseRandomItems(const std::string lines[], int lineCount) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    const int numItemsToChoose = 42;
    std::string randomItems[numItemsToChoose];

    for (int i = 0; i < numItemsToChoose; i++) {
        std::size_t randomIndex = std::rand() % lineCount;
        randomItems[i] = lines[randomIndex];
    }

    insertionSort(randomItems, numItemsToChoose); // sort the random items

    // Create and fill the hash table
    HashTable hashTable;
    for (int i = 0; i < lineCount; i++) {
        hashTable.insert(lines[i], lines[i]);
    }

    int totalLinearComparisons = 0; // comparisons made with linear search
    int totalBinaryComparisons = 0; // comparisons made with binary search
    int totalHashComparisons = 0; // comparisons made with hashing

    std::cout << "===============================================" << std::endl;
    std::cout << "Linear and Binary Search" << std::endl;
    std::cout << "===============================================" << std::endl;
    for (int i = 0; i < numItemsToChoose; i++) {
        std::cout << "Searching for: " << randomItems[i] << std::endl;

        int linearComparisons = 0;
        bool linearFound = linearSearch(randomItems[i], lines, lineCount, linearComparisons);
        totalLinearComparisons += linearComparisons;

        int binaryComparisons = 0;
        bool binaryFound = binarySearch(randomItems[i], lines, lineCount, binaryComparisons);
        totalBinaryComparisons += binaryComparisons;

        // Retrieve item from the hash table and count comparisons
        std::string value;
        int hashComparisons;
        bool hashFound = hashTable.get(randomItems[i], value, hashComparisons);
        totalHashComparisons += hashComparisons;

        std::cout << "Linear Search Comparisons: " << linearComparisons << std::endl;
        std::cout << "Binary Search Comparisons: " << binaryComparisons << std::endl;
        std::cout << "Hash Table Comparisons: " << hashComparisons << std::endl;

        std::cout << "-------" << std::endl;
    }

    // Calculate and print the average comparisons with two decimal places
    double averageLinearComparisons = static_cast<double>(totalLinearComparisons) / numItemsToChoose;
    double averageBinaryComparisons = static_cast<double>(totalBinaryComparisons) / numItemsToChoose;
    double averageHashComparisons = static_cast<double>(totalHashComparisons) / numItemsToChoose;
    std::cout << "===============================================" << std::endl;
    std::cout << "Average Linear Search Comparisons: " << std::fixed << std::setprecision(2) << averageLinearComparisons << std::endl;
    std::cout << "Average Binary Search Comparisons: " << std::fixed << std::setprecision(2) << averageBinaryComparisons << std::endl;
    std::cout << "Average Hash Table Comparisons: " << std::fixed << std::setprecision(2) << averageHashComparisons << std::endl;
    std::cout << "===============================================" << std::endl;
}

int main() {
    std::ifstream inputFile("magic_items.txt");
    if (!inputFile.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return 1;
    }

    const int maxLines = 666;
    std::string lines[maxLines];

    int lineCount = 0;

    std::string line;
    while (lineCount < maxLines && std::getline(inputFile, line)) {
        // Remove whitespace and convert to lowercase
        std::transform(line.begin(), line.end(), line.begin(), ::tolower);
        line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());

        lines[lineCount] = line;
        lineCount++;
    }

    inputFile.close();

    // Call function to get random items and seemingly do everything else
    chooseRandomItems(lines, lineCount);

    return 0;
}
