//  𝞢𝞣𝑯𝞓𝞜
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>





/// KNAPSACK PORTION OF CODE BELOW ///

// constructs the knapsack object 🎒
struct Knapsack {
    int knapsackNumber;
    int capacity;
    double totalValue;
};

// construction of the spice object 
struct Spice {
    std::string name;
    double total_price;
    int qty;
    double price_per_unit;
};

// Insertion Sort Function for spices based on price per unit in descending order
void insertionSort(std::vector<Spice>& spices) {
    for (size_t i = 1; i < spices.size(); ++i) {
        Spice valueToInsert = spices[i];
        int j = i - 1;
        while (j >= 0 && spices[j].price_per_unit < valueToInsert.price_per_unit) {
            spices[j + 1] = spices[j];
            j = j - 1;
        }
        spices[j + 1] = valueToInsert;
    }
}

// Function to open the spice file and parse it (Parsing helped by ChatGPT 🤖)
void parseSpice(const std::string& filename, std::vector<Spice>& spices, std::vector<Knapsack>& knapsacks) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return;
    }

    std::string line;
    int currentKnapsackNumber = 1; // knapsack count
    while (std::getline(file, line)) {
        if (line.empty() || line.compare(0, 2, "--") == 0) {
            continue; // ignore empty lines or comments
        } else if (line.find("knapsack capacity") != std::string::npos) {
            std::istringstream iss(line);
            std::string token;

            while (iss >> token) {
                if (token == "capacity") {
                    iss >> token; // "="
                    int capacity;
                    iss >> capacity;
                    Knapsack newKnapsack;
                    newKnapsack.knapsackNumber = currentKnapsackNumber++;
                    newKnapsack.capacity = capacity;
                    knapsacks.push_back(newKnapsack);
                    break;
                }
            }
        } else {
            std::istringstream iss(line);
            std::string token;

            // Parse the line to get spice details
            std::string spiceName;
            double totalPrice = 0.0;
            int quantity = 0;

            while (iss >> token) {
                if (token == "spice") {
                    iss >> token; // "name"
                    iss >> token; // "="
                    iss >> token; // spice name
                    spiceName = token.substr(0, token.size() - 1); // Remove the semicolon
                } else if (token == "total_price") {
                    iss >> token; // "="
                    std::string priceStr;
                    iss >> priceStr;
                    totalPrice = std::stod(priceStr.substr(0, priceStr.size() - 1)); // Remove the semicolon
                } else if (token == "qty") {
                    iss >> token; // "="
                    std::string qtyStr;
                    iss >> qtyStr;
                    quantity = std::stoi(qtyStr.substr(0, qtyStr.size() - 1)); // Remove the semicolon
                }
            }
            // if not empty, success 😎
            if (!spiceName.empty()) {
                Spice newSpice;
                newSpice.name = spiceName;
                newSpice.total_price = totalPrice;
                newSpice.qty = quantity;
                newSpice.price_per_unit = totalPrice / quantity;
                spices.push_back(newSpice);
            }
        }
    }

    file.close();
}

int main() {
    std::vector<Spice> spices;
    std::vector<Knapsack> knapsacks;
    parseSpice("spice.txt", spices, knapsacks);

    insertionSort(spices); // Sort spices based on price per unit

    int totalQuantity = 0; // Total available quantity of spices

    // Calculate the total quantity of available spices
    for (size_t i = 0; i < spices.size(); ++i) {
        const Spice& spice = spices[i];
        totalQuantity += spice.qty;
    }

    for (size_t i = 0; i < knapsacks.size(); ++i) {
        Knapsack& knapsack = knapsacks[i];
        std::cout << "Knapsack of capacity " << knapsack.capacity << " is worth ";

        int remainingCapacity = knapsack.capacity;
        std::vector<std::string> contents;
        std::vector<Spice> availableSpices = spices; // Create a copy for this knapsack

        // Loop through le spices and fill the current knapsack
        for (size_t j = 0; j < availableSpices.size(); ++j) {
            Spice& spice = availableSpices[j];

            while (remainingCapacity > 0 && spice.qty > 0) {
                if (spice.qty <= remainingCapacity) {
                    contents.push_back(std::to_string(spice.qty) + " scoops of " + spice.name);
                    remainingCapacity -= spice.qty;
                    knapsack.totalValue += spice.total_price;
                    spice.qty = 0; // All scoops used for this knapsack
                } else {
                    contents.push_back(std::to_string(remainingCapacity) + " scoops of " + spice.name);
                    spice.qty -= remainingCapacity;
                    knapsack.totalValue += remainingCapacity * spice.price_per_unit;
                    remainingCapacity = 0; // Knapsack filled to capacity for this knapsack
                }
            }
        }

        // Output knapsack contents and total value 
        std::cout << knapsack.totalValue << " quatloos and contains ";
        for (size_t j = 0; j < contents.size(); ++j) {
            std::cout << contents[j];
            if (j != contents.size() - 1) {
                std::cout << " and ";
            }
        }
        std::cout << "." << std::endl;
    }

    return 0;
}
