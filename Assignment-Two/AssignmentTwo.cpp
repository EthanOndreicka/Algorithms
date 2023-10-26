#include <iostream>
#include <fstream>
#include <string>

// File Input from ChatGPT
int main() {
    std::ifstream inputFile("magic_items.txt");
    if (!inputFile.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return 1;
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

    // Print the lines stored in the array
    for (int i = 0; i < lineCount; i++) {
        std::cout << lines[i] << std::endl;
    }

    return 0;
}