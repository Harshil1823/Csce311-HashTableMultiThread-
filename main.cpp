#include <iostream>
#include <fstream>
#include <pthread.h>
#include <sstream>
#include "HashTable.h" 

// Structure to hold operation details
struct Operation {
    char type;
    int key;
    std::string value;
};

HashTable myHashTable;  // Create an instance of custom HashTable
pthread_mutex_t mapMutex = PTHREAD_MUTEX_INITIALIZER;
// Default number of threads
int numOfThreads = 1;

// Function to process a single operation
void processOperation(const Operation& operation, std::ofstream& outputFile) {
    pthread_mutex_lock(&mapMutex);

    switch (operation.type) {
        case 'I':
            myHashTable.insert(operation.key, operation.value);
            outputFile << "OK\n";
            break;
        case 'D':
            if (myHashTable.remove(operation.key)) {
                outputFile << "OK\n";
            } else {
                outputFile << "Fail\n";
            }
            break;
        case 'L':
            outputFile << myHashTable.lookup(operation.key) << '\n';
            break;
        default:
            break;
    }

    pthread_mutex_unlock(&mapMutex);
}

// Thread function to process operations
// Thread function to process operations
void* processOperationsThread(void* arg) {
    std::pair<std::ifstream*, std::string>* threadArgs = static_cast<std::pair<std::ifstream*, std::string>*>(arg);
    std::ifstream* inputFile = threadArgs->first;
    std::string outputFilePath = threadArgs->second;
    std::string line;

    // Open the output file using the specified path
    std::ofstream outputFile(outputFilePath);
    // outputFile << "Using " << numOfThreads << " threads\n" << std::endl;

    while (std::getline(*inputFile, line)) {
        Operation operation;
        std::istringstream iss(line);

        if(line[0] == 'N') {
            iss >> line >> numOfThreads;
            outputFile << "Using " << numOfThreads << " threads\n" << std::endl;
            continue; // skip to next iteration
        }
        iss >> operation.type;

        switch (operation.type) {
            case 'I':
                iss >> operation.key >> operation.value;
                break;
            case 'D':
            case 'L':
                iss >> operation.key;
                break;
            default:
                break;
        }
        processOperation(operation, outputFile);  // Pass outputFile to processOperation
    }

    outputFile.close(); // Close the output file before the thread ends

    return nullptr;
}


int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input file relative path> <output file relative path>\n";
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    std::ofstream outputFile(argv[2]);
    std::pair<std::ifstream*, std::string> threadArgs(&inputFile, argv[2]);

    if (!inputFile.is_open() || !outputFile.is_open()) {
        std::cerr << "Error opening files.\n";
        return 1;
    }
    // Increment the thread counter
    numOfThreads++;
    pthread_t thread;
    pthread_create(&thread, nullptr, processOperationsThread, &threadArgs);

    pthread_join(thread, nullptr);

    return 0;
}
