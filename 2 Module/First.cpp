#include <iostream>
#include <cassert>
#include <vector>

enum status {
    DELETE,
    EMPTY,
    DATA
};

struct StringHasher {
    unsigned int operator()(const std::string &str) const {
        unsigned int hash = 0;
        for (char i: str) {
            hash = hash * 17 + i;
        }
        return hash;
    }
};


template<class T, class H>
class HashTable {
public:
    explicit HashTable(const H &hasher_);

    HashTable(const HashTable &table) = delete;

    HashTable &operator=(const HashTable &table) = delete;

    ~HashTable();

    bool Has(const T &key) const;

    bool Add(const T &key);

    bool Delete(const T &key);

private:
    struct HashTableCell {
        T data;
        int status;
        unsigned int Hash;

        HashTableCell() : Hash(0), status(EMPTY) {}
        explicit HashTableCell(const T &data_, unsigned int Hash_) : data(data_), status(DATA), Hash(Hash_) {}
    };

    H hasher;
    int keysCount;
    std::vector<HashTableCell *> table;

    void growTable();
};

template<class T, class H>
HashTable<T, H>::HashTable(const H &hasher_) {

}

template<class T, class H>
HashTable<T, H>::~HashTable() {

}

template<class T, class H>
bool HashTable<T, H>::Has(const T &key) const {
    //del - идем дальше
    //data data== ? true : дальше
    //empty return false

    return false;
}

template<class T, class H>
bool HashTable<T, H>::Add(const T &key) {
    //del - запоминаем позицию и  идем дальше
    //data data== ? false : дальше
    //empty заполняем в del позицию или сюда

    return false;
}

template<class T, class H>
bool HashTable<T, H>::Delete(const T &key) {

    //del - идем дальше
    //data data== ? помечаем del true : дальше
    //empty return false

    return false;
}

template<class T, class H>
void HashTable<T, H>::growTable() {

}

int main() {
    StringHasher hasher;
    HashTable<std::string, StringHasher> table(hasher);
    char operation = 0;
    std::string data;
    while (std::cin >> operation >> data) {
        switch (operation) {
            case '+' :
                std::cout << (table.Add(data) ? "OK" : "FAIL") << std::endl;
                break;
            case '-' :
                std::cout << (table.Delete(data) ? "OK" : "FAIL") << std::endl;
                break;
            case '?' :
                std::cout << (table.Has(data) ? "OK" : "FAIL") << std::endl;
                break;
            default:
                assert(false);

        }
    }
    return 0;
}