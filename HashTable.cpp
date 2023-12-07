#include <mutex>
#include "HashTable.h"
using std::mutex;

HashTable::HashTable() {
    table = new HashNode*[TABLE_SIZE]();
}

HashTable::~HashTable() {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        HashNode* current = table[i];
        while (current) {
            HashNode* next = current->next;
            delete current;
            current = next;
        }
    }
    delete[] table;
}

int HashTable::hashFunction(int key) {
    return key % TABLE_SIZE;
}

void HashTable::insert(int key, const std::string& value) {
    int index = hashFunction(key);

    // Lock the mutex before modifying shared data
    std::lock_guard<std::mutex> lock(mutex);

    HashNode* newNode = new HashNode(key, value);
    newNode->next = table[index];
    table[index] = newNode;
}

bool HashTable::remove(int key) {
    int index = hashFunction(key);

    // Lock the mutex before modifying shared data
    std::lock_guard<std::mutex> lock(mutex);

    HashNode* current = table[index];
    HashNode* prev = nullptr;

    while (current) {
        if (current->key == key) {
            if (prev) {
                prev->next = current->next;
            } else {
                table[index] = current->next;
            }
            delete current;
            return true;
        }
        prev = current;
        current = current->next;
    }

    return false;
}

std::string HashTable::lookup(int key) {
    int index = hashFunction(key);

    // Lock the mutex before accessing shared data
    std::lock_guard<std::mutex> lock(mutex);

    HashNode* current = table[index];
    while (current) {
        if (current->key == key) {
            return current->value;
        }
        current = current->next;
    }

    return "No " + std::to_string(key);
}
