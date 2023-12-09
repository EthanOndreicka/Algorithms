//  𝞢𝞣𝑯𝞓𝞜

// I don't know if my brain isn't working but I can't get each graph to print individually

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <limits>

// SSSP CODE DOWN BELOW 🚙

// contructs each vertex
struct Vertex {
    int id;
    bool processed;
    std::vector<Vertex*> neighbors;
    std::vector<int> weights;
    long long distance; //  store distance for Bellman-Ford
    Vertex* predecessor; // store predecessor for path reconstruction
    Vertex(int _id) : id(_id), processed(false), distance(std::numeric_limits<int>::max()), predecessor(nullptr) {}
};


// creates the graph class
class Graph {
public:
    std::unordered_map<int, Vertex*> vertices;

    void addVertex(int id) {
        if (vertices.find(id) == vertices.end()) {
            vertices[id] = new Vertex(id);
        }
    }

    // adds edge between vertices w/ weights
    void addEdge(int src, int dest, int weight) {
        if (vertices.find(src) == vertices.end() || vertices.find(dest) == vertices.end()) {
            std::cerr << "Vertices not found for edge addition." << std::endl;
            return;
        }

        vertices[src]->neighbors.push_back(vertices[dest]);
        vertices[src]->weights.push_back(weight);
    }
    // clear graph for next one
    void clearVertices() {
        for (std::unordered_map<int, Vertex*>::iterator it = vertices.begin(); it != vertices.end(); ++it) {
            delete it->second;
        }
        vertices.clear();
    }

    void initializeSingleSource(Vertex* source) {
        for (std::unordered_map<int, Vertex*>::iterator it = vertices.begin(); it != vertices.end(); ++it) {
            it->second->distance = std::numeric_limits<int>::max();
            it->second->predecessor = nullptr;
        }
        source->distance = 0;
    }

    // relaxxxxxxxxxxxxxxx
    void relax(Vertex* u, Vertex* v, int weight) {
        if (v->distance > static_cast<long long>(u->distance) + weight) {
            v->distance = static_cast<long long>(u->distance) + weight;
            v->predecessor = u;
        }
    }

    void bellmanFord(Vertex* source, int maxIterations) {
        initializeSingleSource(source);

        for (size_t i = 0; i < maxIterations; ++i) {
            for (std::unordered_map<int, Vertex*>::iterator it = vertices.begin(); it != vertices.end(); ++it) {
                Vertex* u = it->second;
                for (size_t j = 0; j < u->neighbors.size(); ++j) {
                    relax(u, u->neighbors[j], u->weights[j]);
                }
            }
        }
    }

    void printShortestPaths(Vertex* source) {
        for (std::unordered_map<int, Vertex*>::iterator it = vertices.begin(); it != vertices.end(); ++it) {
            if (it->second != source) {
                std::cout << "Path from " << source->id << " to " << it->second->id << " cost is "
                        << it->second->distance << " path is ";
                printPath(source, it->second);
                std::cout << std::endl;
            }
        }
    }

    // prints the path taken for the shortest path
    void printPath(Vertex* source, Vertex* dest) {
        if (dest == source) {
            std::cout << source->id;
            return;
        } else if (dest->predecessor == nullptr) {
            std::cout << "No path from " << source->id << " to " << dest->id << " exists.";
            return;
        } else {
            printPath(source, dest->predecessor);
            std::cout << " -> " << dest->id;
        }
    }
    
};

// file reader for the graphs2.txt
void parseInputFile(const std::string& filename, Graph& graph) {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        return;
    }

    std::string line;
    bool newGraph = true; // Flag to indicate a new graph
    while (std::getline(inputFile, line)) {
        if (line.empty() || line.substr(0, 2) == "--") {
            newGraph = true; // Indicates a new graph
            continue;
        }

        if (newGraph) {
            graph.clearVertices(); // Clear existing graph data for the new graph
            newGraph = false; // Reset flag
        }

        if (line.substr(0, 3) == "add") {
            std::istringstream iss(line);
            std::string command;
            iss >> command;

            if (command == "add") {
                std::string subcommand;
                iss >> subcommand;

                if (subcommand == "vertex") {
                    int vertexId;
                    iss >> vertexId;
                    graph.addVertex(vertexId);
                } else if (subcommand == "edge") {
                    int src, dest, weight;
                    char dash;
                    iss >> src >> dash >> dest >> weight;
                    graph.addEdge(src, dest, weight);
                }
            }
        }
    }

    inputFile.close();
}






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
    std::cout << "===============================================================" << std::endl;
    std::cout << "==================== Bellman-Ford SSSP ========================" << std::endl;
    std::cout << "===============================================================" << std::endl;
    Graph graph;
    parseInputFile("graphs2.txt", graph);
    // Run Bellman-Ford algorithm from vertex 1
    Vertex* source = graph.vertices[1];
    graph.bellmanFord(source, 100);

    // Print shortest paths from vertex 1 to all other vertices
    graph.printShortestPaths(source);
    std::cout << "===============================================================" << std::endl;
    std::cout << "===================== Fractional Knapsack =====================" << std::endl;
    std::cout << "===============================================================" << std::endl;
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
    std::cout << "===============================================================" << std::endl;
    

    return 0;
}