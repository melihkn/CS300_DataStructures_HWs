/*
    Melih Kaan Þahinbaþ CS300 HW3 HashTable class
*/

#include <string>
#include <iostream>
#include <vector>
#include <cmath>


struct DocumentItem {
private:
    std::string documentName;
    int count;

public:
    void incrementCount() { count++; }
    int getCount() { return count; }

    DocumentItem(const std::string& documentName = "", const int& count = 1)
        : documentName(documentName), count(count) {}
};

template <typename T>
struct WordItem {
    std::string word;
    T list;

    WordItem() {}

    WordItem(std::string word)
        : word(word)
    {    }

    ~WordItem() {}
};


template <class Key, class Value>
class HashTable {
public:
    HashTable() {
        arr.resize(53);
        for (int i = 0; i < arr.size(); i++) {
            arr[i].info = EMPTY;
        }
    }

    HashTable(const Value& notFound,
        int size)
        : ITEM_NOT_FOUND(notFound), arr(nextPrime(size))
    {
        makeEmpty();
    }

    HashTable(const HashTable& rhs)
        : ITEM_NOT_FOUND(rhs.ITEM_NOT_FOUND),
        arr(rhs.arr), currentSize(rhs.currentSize) { }

    Value* find(const Key& key) const {
        int currentPos = findPos(key);
        if (isActive(currentPos))
            return arr[currentPos].HashedObj;

        return nullptr;
    }

    void makeEmpty();
    void insert(const Key& key, Value* data) {
        // Insert x as active
        int currentPos = findPos(key);

        if (isActive(currentPos))
            return;

        arr[currentPos] = HashEntry(key, data, ACTIVE);

        // enlarge the hash table if necessary 
        if (++currentSize >= arr.size() * 0.9) {
            try {
                int size = arr.size();
                rehash();
                std::cout << "rehashed...\nprevious table size:" << size << ", new table size: " << arr.size() <<
                    ", current unique word count: " << currentSize + 1 << ", current load factor: " << getLoadFactor() << std::endl;
            }
            catch (const std::exception& e) {
                std::cerr << "Rehashing failed: " << e.what() << std::endl;
                throw;  // re-throw the exception after logging
            }
        }
    }

    void remove(const Value& x) {
        int currentPos = findPos(x);
        if (isActive(currentPos))
            arr[currentPos].info = DELETED;
    }


    const HashTable& operator=(const HashTable& rhs);

    enum EntryType { ACTIVE, EMPTY, DELETED };

    int getCurrentSize() { return currentSize; }
    double getLoadFactor() { return (double)currentSize / arr.size(); }

private:
    struct HashEntry
    {
        Key key;
        Value* HashedObj;
        EntryType info;

        HashEntry() {}

        HashEntry(Key key, Value* data, EntryType info)
            : key(key), HashedObj(data), info(info)
        {}
    };

    std::vector<HashEntry> arr;
    int currentSize;
    const Value ITEM_NOT_FOUND;

    bool isActive(int currentPos) const {
        if (currentPos >= 0 && currentPos < arr.size()) {
            return arr[currentPos].info == ACTIVE;
        }
        else {
            // Handle out-of-bounds access (e.g., by returning false)
            return false;
        }
    }

    int findPos(const Key& x) const {
        int collisionNum = 0;
        int currentPos = hash(x, arr.size());

        currentPos = currentPos % arr.size();

        while (currentPos < arr.size() && arr[currentPos].info != EMPTY && arr[currentPos].key != x) {
            currentPos += pow(++collisionNum, 2);  //add the difference
            currentPos = currentPos % arr.size();              // if necessary
        }
        return currentPos;
    }

    size_t nextPrime(size_t n) {
        while (!isPrime(n))
            n++;
        return n;
    }

    // Function to check if a number is prime
    bool isPrime(size_t n) {
        if (n <= 1)
            return false;
        if (n <= 3)
            return true;
        if (n % 2 == 0 || n % 3 == 0)
            return false;
        for (size_t i = 5; i * i <= n; i = i + 6)
            if (n % i == 0 || n % (i + 2) == 0)
                return false;
        return true;
    }

    void rehash() {
        std::vector<HashEntry> oldArray = arr;

        // Create new double-sized, empty table
        arr.resize(nextPrime(2 * oldArray.size()));
        for (int j = 0; j < arr.size(); j++)
            arr[j].info = EMPTY;

        // Copy table over
        currentSize = 0;
        for (int i = 0; i < oldArray.size(); i++)
            if (oldArray[i].info == ACTIVE)
                insert(oldArray[i].key, oldArray[i].HashedObj);

    }

    int hash(const Key& key, unsigned int arr_size) const {

        int hash_val = 0;

        //if constexpr (std::is_same<Key, std::string>) {
            for (int i = 0; i < key.length(); i++) {
                hash_val += (i + 1) * key[i];
            }
        //}

        return hash_val;
    }

};
