#include <iostream>
#include <fstream>
#include <string>

// Constructs the node
struct Node {
    char data;
    Node* next;

    Node(char ch) : data(ch), next(nullptr) {}
};

// Split the array into two subarrays and return the pivot value
int split(std::string arr[], int low, int high, int& comparisonCount) {
    // Choose the pivot as the middle element
    int middle = low + (high - low) / 2;
    std::string pivot = arr[middle];

    // Move the pivot element to the end
    std::swap(arr[middle], arr[high]);

    int i = low - 1; // Index of the smaller element

    for (int j = low; j < high; j++) {
        comparisonCount++; // Count each comparison
        if (arr[j] <= pivot) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }

    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

// Quick sort function with comparison counter
void quickSort(std::string arr[], int low, int high, int& comparisonCount) {
    if (low < high) {
        int pivotIndex = split(arr, low, high, comparisonCount);

        // Recursively sort the subarrays
        quickSort(arr, low, pivotIndex - 1, comparisonCount);
        quickSort(arr, pivotIndex + 1, high, comparisonCount);
    }
}

// Creates the merge function
void merge(std::string arr[], int start, int middle, int end, int& comparisonCount) {
    int leftSize = middle - start + 1;
    int rightSize = end - middle;

    // Create temporary arrays for left and right
    std::string left[leftSize];
    std::string right[rightSize];

    for (int i = 0; i < leftSize; i++) {
        left[i] = arr[start + i];
    }
    for (int j = 0; j < rightSize; j++) {
        right[j] = arr[middle + 1 + j];
    }

    // Merge the two halves back into the original array
    int i = 0, j = 0, k = start;
    while (i < leftSize && j < rightSize) {
        comparisonCount++;
        if (left[i] <= right[j]) {
            arr[k] = left[i];
            i++;
        } else {
            arr[k] = right[j];
            j++;
        }
        k++;
    }

    // Copy remaining elements of left[] and right[] if any
    while (i < leftSize) {
        arr[k] = left[i];
        i++;
        k++;
    }
    while (j < rightSize) {
        arr[k] = right[j];
        j++;
        k++;
    }
}

// Merge sort function
void mergeSort(std::string arr[], int start, int end, int& comparisonCount) {
    if (start < end) {
        int middle = (start + end) / 2;

        // Recursively sort the left and right halves
        mergeSort(arr, start, middle, comparisonCount);
        mergeSort(arr, middle + 1, end, comparisonCount);

        // Merge the sorted halves
        merge(arr, start, middle, end, comparisonCount);
    }
}

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

    std::cout << "=====================================" << std::endl;
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

    // reshuffle the array
    knuthShuffle(shuffled_lines, lineCount);

    // Insertion sort
    int insertionSortCount = 0;
    for (int i = 1; i < lineCount; i++) {
        std::string valueToInsert = shuffled_lines[i];
        int j = i - 1;

        while (j >= 0 && shuffled_lines[j] > valueToInsert) {
            shuffled_lines[j + 1] = shuffled_lines[j];
            j = j - 1;
            insertionSortCount++;
        }
        shuffled_lines[j + 1] = valueToInsert;
    }
    std::cout << "Total number of sorts using Insertion Sort: " << insertionSortCount << std::endl;
    std::cout << "=====================================" << std::endl;

    // reshuffle the array againnnnnnnnnn
    knuthShuffle(shuffled_lines, lineCount);

    int mergeSortComparisonCount = 0;
    mergeSort(shuffled_lines, 0, lineCount - 1, mergeSortComparisonCount);

    // Output the sorted lines
   // std::cout << "Using Merge Sort:" << std::endl;
   // for (int i = 0; i < lineCount; i++) {
    //    std::cout << shuffled_lines[i] << std::endl;
   // }

    std::cout << "Total number of sorts using Merge Sort: " << mergeSortComparisonCount << std::endl;
    std::cout << "=====================================" << std::endl;

    // reshuffle the array againnnnnnnnnnnnnnnnnnnnnnnnnnnn
    // Personally I think we should've only done Monkey sort
    knuthShuffle(shuffled_lines, lineCount);

    int quickSortComparisonCount = 0;
    // Call quickSort to sort the array and count comparisons
    quickSort(shuffled_lines, 0, lineCount - 1, quickSortComparisonCount);

    // Output the sorted lines
    //std::cout << "Using Quick Sort:" << std::endl;
    //for (int i = 0; i < lineCount; i++) {
    //    std::cout << shuffled_lines[i] << std::endl;
    //}
    std::cout << "Total number of comparisons using Quick Sort: " << quickSortComparisonCount << std::endl;
    std::cout << "=====================================" << std::endl;

    return 0;
}
