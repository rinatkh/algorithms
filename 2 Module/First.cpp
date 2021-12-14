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

    bool Has(const T &data) const;

    bool Add(const T &data);

    bool Delete(const T &data);

private:
    struct HashTableCell {
        T data;
        int status;
        unsigned int Hash;

        HashTableCell() : Hash(0), status(EMPTY) {}

        explicit HashTableCell(const T &data_, unsigned int Hash_) : data(data_), status(DATA),
                                                                     Hash(Hash_) {}
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
bool HashTable<T, H>::Has(const T &data) const {
    //del - идем дальше
    //data data== ? true : дальше
    //empty return false

    return false;
}

template<class T, class H>
bool HashTable<T, H>::Add(const T &data) {
    //del - запоминаем позицию и  идем дальше
    //data data== ? false : дальше
    //empty заполняем в del позицию или сюда

    if (keysCount >= table.size() * 0.75)
        growTable();

    unsigned int hash = hasher(data);
    unsigned int abs_hash = hash % table.size();
    HashTableCell *cell = table[hash];

    unsigned int first_del = -1;
    for (int i = 0; i < table.size() && table[hash] != nullptr; i++) {
        if (table[hash]->data == data) {
            return false;
        }
        else if ((table[hash]->status == DELETE) && (first_del == -1)) {
            first_del = hash;
        }
        else if(table[hash]->status == EMPTY) {
            table[hash]->data = data;
            keysCount++;
            return true;
        }
        hash = (hash + i + 1) % table.size();
    }

    table[hash] = new HashTableCell(data, hash);
    keysCount++;
    return true;
}

template<class T, class H>
bool HashTable<T, H>::Delete(const T &data) {

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