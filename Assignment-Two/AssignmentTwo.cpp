#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <algorithm>
#include <cctype>
#include <cstddef>

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


// Choose random item function partially from ChatGPT
void chooseRandomItems(const std::string lines[], int lineCount) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    const int numItemsToChoose = 42;
    std::string randomItems[numItemsToChoose];

    for (int i = 0; i < numItemsToChoose; i++) {
        std::size_t randomIndex = std::rand() % lineCount;
        randomItems[i] = lines[randomIndex];
    }

    for (int i = 1; i < numItemsToChoose; i++) {
        std::string valueToInsert = randomItems[i];
        int j = i - 1;

        while (j >= 0 && randomItems[j] > valueToInsert) {
            randomItems[j + 1] = randomItems[j];
            j = j - 1;
        }
        randomItems[j + 1] = valueToInsert;
    }

    int totalComparisons = 0; // total comparisons made for all 42 searches

    std::cout << "===============================================" << std::endl;
    std::cout << "   Linear Search    " << std::endl;
    std::cout << "===============================================" << std::endl;
    for (int i = 0; i < numItemsToChoose; i++) {
        std::cout << "Searching for: " << randomItems[i] << std::endl;
        int comparisons = 0;
        bool found = linearSearch(randomItems[i], lines, lineCount, comparisons);
        totalComparisons += comparisons;
        std::cout << "Comparisons: " << comparisons << std::endl;
    }

    // get average comparisons
    std::cout << "===============================================" << std::endl;
    double averageComparisons = static_cast<double>(totalComparisons) / numItemsToChoose;
    // outputs and sets to 2 decimal places
    std::cout << "Average Comparisons for Linear Search : " << std::fixed << std::setprecision(2) << averageComparisons << std::endl;
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
        // remove whitespace and lower each line
        std::transform(line.begin(), line.end(), line.begin(), ::tolower);
        line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());

        lines[lineCount] = line;
        lineCount++;
    }

    inputFile.close();

    // call function to get random items
    chooseRandomItems(lines, lineCount);

    return 0;
}
