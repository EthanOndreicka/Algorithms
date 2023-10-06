#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>

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

// Custom stack implementation using linked list
template <typename T>
class MyStack {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& item) : data(item), next(nullptr) {}
    };
    Node* top;

public:
    MyStack() : top(nullptr) {}

    void push(const T& item) {
        Node* newNode = new Node(item);
        newNode->next = top;
        top = newNode;
    }

    void pop() {
        if (top) {
            Node* temp = top;
            top = top->next;
            delete temp;
        }
    }

    T& peek() const {
        if (top) {
            return top->data;
        }
        throw std::out_of_range("Stack is empty");
    }

    bool empty() const {
        return top == nullptr;
    }
};

// Custom queue implementation using linked list
template <typename T>
class MyQueue {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& item) : data(item), next(nullptr) {}
    };
    Node* front;
    Node* rear;

public:
    MyQueue() : front(nullptr), rear(nullptr) {}

    void enqueue(const T& item) {
        Node* newNode = new Node(item);
        if (rear) {
            rear->next = newNode;
        } else {
            front = newNode;
        }
        rear = newNode;
    }

    void dequeue() {
        if (front) {
            Node* temp = front;
            front = front->next;
            delete temp;
            if (!front) {
                rear = nullptr;
            }
        }
    }

    T& frontElement() const {
        if (front) {
            return front->data;
        }
        throw std::out_of_range("Queue is empty");
    }

    bool empty() const {
        return front == nullptr;
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

    int lineCount = 0;
    int palindromeCount = 0;

    std::string line;
    while (lineCount < maxLines && std::getline(inputFile, line)) {
        // Changes each line to lowercase and removes whitespace
        std::transform(line.begin(), line.end(), line.begin(), ::tolower);
        line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());

        // Initialize the linked list to nullptr
        lines[lineCount] = nullptr;

        // Create a linked list of nodes for the characters in the line
        Node* current = nullptr;
        for (size_t i = 0; i < line.length(); i++) {
            char ch = line[i];
            push(lines[lineCount], ch); // Push each character onto the linked list
        }

        // Compare nodes while popping from a stack and queue
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
            char queueFront = charQueue.frontElement();

            if (stackTop != queueFront) {
                isPalindrome = false;
                break; // Not a palindrome, no need to continue checking
            }

            charStack.pop();
            charQueue.dequeue();
        }

        if (isPalindrome) {
            palindromeCount++;
            std::cout << "Line " << lineCount + 1 << " is a palindrome." << std::endl;
        }

        // Free memory used by nodes
        while (lines[lineCount] != nullptr) {
            Node* temp = lines[lineCount];
            lines[lineCount] = lines[lineCount]->next;
            delete temp;
        }

        lineCount++;
    }

    inputFile.close();

    std::cout << "Total number of palindromes: " << palindromeCount << std::endl;

    return 0;
}
