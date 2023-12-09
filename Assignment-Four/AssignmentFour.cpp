#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>


// constructs the knapsack object
struct Knapsack {
    int knapsackNumber;
    int capacity;
};

// construction of the spice object 
struct Spice {
    std::string name;
    double total_price;
    int qty;
    double price_per_unit;
};

// function to open the spice file and parse it (Parsing helped by ChatGPT)
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

// main function
int main() {
    std::vector<Spice> spices;
    std::vector<Knapsack> knapsacks;
    parseSpice("spice2.txt", spices, knapsacks);

    // printing spices for testing (using spice2.txt for personal testing)
    std::cout << "Spices Information:" << std::endl;
    for (size_t i = 0; i < spices.size(); ++i) {
        const Spice& spice = spices[i];
        std::cout << "Spice Name: " << spice.name << std::endl;
        std::cout << "Total Price: " << spice.total_price << std::endl;
        std::cout << "Quantity: " << spice.qty << std::endl;
        std::cout << "Price per Unit: " << std::fixed << std::setprecision(2) << spice.price_per_unit << std::endl;
        std::cout << "------------------" << std::endl;
    }

    // printing knapsacks for testing (using spice2.txt for personal testing)
    std::cout << "Knapsacks Information:" << std::endl;
    for (size_t i = 0; i < knapsacks.size(); ++i) {
        const Knapsack& knapsack = knapsacks[i];
        std::cout << "Knapsack Number: " << knapsack.knapsackNumber << std::endl;
        std::cout << "Capacity: " << knapsack.capacity << std::endl;
        std::cout << "------------------" << std::endl;
    }

    return 0;
}
