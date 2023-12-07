#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include <string>
#include <mutex>
using std::endl;
using std::cout;
using std::string;

struct HashNode {
    int key;
    string value;
    HashNode* next;

    HashNode(int k, const string& v) : key(k), value(v), next(nullptr) {}
};

class HashTable {
public:
    HashTable();
    ~HashTable();

    void insert(int key, const string& value);
    bool remove(int key);
    string lookup(int key);

private:
    static const int TABLE_SIZE = 4194304;  // Adjust size based on your needs
    HashNode** table;

    int hashFunction(int key);
};

#endif // HASHTABLE_H
