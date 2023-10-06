#include <iostream>
#include <fstream>
#include <string>

// Constructs the node
struct Node {
    char data;
    Node* next;

    Node(char ch) : data(ch), next(nullptr) {}
};

// Function to push a node onto a linked list
void push(Node*& head, char ch) {
    Node* newNode = new Node(ch);
    newNode->next = head;
    head = newNode;
}

// Function to perform Knuth shuffle on an array
template <typename T>
void knuthShuffle(T* arr, int size) {
    for (int i = size - 1; i > 0; i--) {
        int j = std::rand() % (i + 1); // Generate a random index from 0 to i (inclusive)
        std::swap(arr[i], arr[j]);
    }
}

// Stack implementation using linked list
template <typename T>
class MyStack {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& item) : data(item), next(nullptr) {}
    };
    Node* head;

public:
    MyStack() : head(nullptr) {}

    void push(const T& item) {
        Node* newNode = new Node(item);
        newNode->next = head;
        head = newNode;
    }

    void pop() {
        if (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    T& peek() const {
        if (head) {
            return head->data;
        }
        throw std::out_of_range("Stack is empty");
    }

    bool empty() const {
        return head == nullptr;
    }
};

// Queue implementation using linked list
template <typename T>
class MyQueue {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& item) : data(item), next(nullptr) {}
    };
    Node* head;
    Node* tail;

public:
    MyQueue() : head(nullptr), tail(nullptr) {}

    void enqueue(const T& item) {
        Node* newNode = new Node(item);
        if (tail) {
            tail->next = newNode;
        } else {
            head = newNode;
        }
        tail = newNode;
    }

    void dequeue() {
        if (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
            if (!head) {
                tail = nullptr;
            }
        }
    }

    T& headElement() const {
        if (head) {
            return head->data;
        }
        throw std::out_of_range("Queue is empty");
    }

    bool empty() const {
        return head == nullptr;
    }
};

// File reader partially from ChatGPT
int main() {
    std::ifstream inputFile("magic_items.txt");
    if (!inputFile.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return 1;
    }

    const int maxLines = 666;
    Node* lines[maxLines]; // Create an array of pointers to nodes
    std::string shuffled_lines[maxLines];

    int lineCount = 0;
    int palindromeCount = 0;

    std::string line;
    while (lineCount < maxLines && std::getline(inputFile, line)) {
        // Changes each line to lowercase and removes whitespace
        std::transform(line.begin(), line.end(), line.begin(), ::tolower);
        line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());

        shuffled_lines[lineCount] = line; // Store the line in the array

        // Initialize the linked list to nullptr
        lines[lineCount] = nullptr;

        // Create a linked list of nodes for the characters in the line
        Node* current = nullptr;
        for (size_t i = 0; i < line.length(); i++) {
            char ch = line[i];
            push(lines[lineCount], ch); // Push each character onto the linked list
        }

        std::srand(static_cast<unsigned int>(std::time(nullptr)));

        // Compare nodes while popping from a stack and queue using template
        MyStack<char> charStack;
        MyQueue<char> charQueue;

        Node* currentNode = lines[lineCount];
        bool isPalindrome = true;

        while (currentNode != nullptr) {
            charStack.push(currentNode->data); // Push onto the stack
            charQueue.enqueue(currentNode->data); // Enqueue into the queue
            currentNode = currentNode->next;
        }

        while (!charStack.empty() && !charQueue.empty()) {
            char stackTop = charStack.peek();
            char queuehead = charQueue.headElement();

            if (stackTop != queuehead) {
                isPalindrome = false;
                break; // Not a palindrome, no need to continue checking
            }

            charStack.pop();
            charQueue.dequeue();
        }

        // Prints out which line in magicitems is a palindrome
        if (isPalindrome) {
            palindromeCount++;
            std::cout << "Line " << lineCount + 1 << " is a palindrome." << std::endl;
        }

        // Free memory used by nodes, sourced from StackOverflow (Took me forever to figure out)
        while (lines[lineCount] != nullptr) {
            Node* temp = lines[lineCount];
            lines[lineCount] = lines[lineCount]->next;
            delete temp;
        }

        lineCount++;
    }

    inputFile.close();

    std::cout << "Total number of palindromes: " << palindromeCount << std::endl;
    // Added lines to keep things organized/seperate
    std::cout << "=====================================" << std::endl;

    knuthShuffle(shuffled_lines, lineCount);
   // std::cout << "Shuffled Lines: " << std::endl;
     // Output the shuffled lines
    //for (int i = 0; i < lineCount; i++) {
     //   std::cout << shuffled_lines[i] << std::endl;
   // }
   // std::cout << "=====================================" << std::endl;

    int selectionSortCount = 0;

    // Selection sort
    for (int i = 0; i < lineCount - 1; i++) {
        int smallestLine = i;
        for (int j = i + 1; j < lineCount; j++) {
            // Counts the amount of comparisons
            selectionSortCount ++;
            if (shuffled_lines[j] < shuffled_lines[smallestLine]) {
                smallestLine = j;
            }
        }
        if (smallestLine != i) {
            std::swap(shuffled_lines[i], shuffled_lines[smallestLine]);
        }
    }

    // Output the sorted lines
    //std::cout << "Using Selection Sort:" << std::endl;
    //for (int i = 0; i < lineCount; i++) {
    //    std::cout << shuffled_lines[i] << std::endl;
    //}
    std::cout << "Total number of sorts using Selection Sort: " << selectionSortCount << std::endl;
    std::cout << "=====================================" << std::endl;



    return 0;
}
