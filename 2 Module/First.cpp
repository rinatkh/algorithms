#include <iostream>
#include <cassert>
#include <vector>

enum status {
    DELETE,
    EMPTY,
    DATA
};

struct StringHasher {
    long long unsigned int operator()(const std::string &str) const {
        long long unsigned int hash = 0;
        for (char i: str)
            hash = hash * 7 + i;

        return hash;
    }
};


template<class T, class H>
class HashTable {
public:
    explicit HashTable(const H &hasher_, size_t initial_size = 8);

    HashTable() = delete;

    HashTable(const HashTable &table) = delete;

    HashTable &operator=(const HashTable &table) = delete;

    ~HashTable();

    bool has(const T &data) const;

    bool add(const T &data);

    bool del(const T &data);

private:
    struct HashTableCell {
        T data;
        int status;
        long long unsigned int hash;

        HashTableCell() : data(), hash(0), status(EMPTY) {}

        explicit HashTableCell(const T &data_, long long unsigned int hash_) : data(data_),
                                                                               status(DATA),
                                                                               hash(hash_) {}

        ~HashTableCell() = default;
    };

    H hasher;
    size_t keys_count;
    std::vector<HashTableCell> table;

    void growTable();
};

template<class T, class H>
HashTable<T, H>::HashTable(const H &hasher_, size_t initial_size) :
        hasher(hasher_),
        keys_count(0),
        table(8) {
    for (auto &i: table)
        i = HashTableCell();
}

template<class T, class H>
HashTable<T, H>::~HashTable() = default;


template<class T, class H>
bool HashTable<T, H>::has(const T &data) const {
    long long unsigned int abs_hash = hasher(data) % table.size();

    for (int i = 0; i < table.size(); ++i) {
        if (table[abs_hash].data == data && table[abs_hash].status == DATA) {
            return true;
        } else if (table[abs_hash].status == EMPTY)
            return false;
        abs_hash = (abs_hash + i + 1) % table.size();
    }
    return false;
}

template<class T, class H>
bool HashTable<T, H>::add(const T &data) {
    if (keys_count >= table.size() * 0.75) {
        growTable();
    }

    long long unsigned int hash = hasher(data);
    long long unsigned int abs_hash = hash % table.size();

    long long unsigned int first_del = -1;
    for (int i = 0; i < table.size(); i++) {
        if (table[abs_hash].data == data && table[abs_hash].status == DATA) {
            return false;
        } else if ((table[abs_hash].status == DELETE) && (first_del == -1)) {
            first_del = abs_hash;
        } else if (table[abs_hash].status == EMPTY) {
            table[abs_hash] = HashTableCell(data, hash);
            keys_count++;
            return true;
        }
        abs_hash = (abs_hash + i + 1) % table.size();
    }
    table[first_del] = HashTableCell(data, hash);
    keys_count++;
    return true;
}

template<class T, class H>
bool HashTable<T, H>::del(const T &data) {
    long long unsigned int hash = hasher(data);
    long long unsigned int abs_hash = hash % table.size();

    for (int i = 0; i < table.size(); i++) {
        if (table[abs_hash].data == data && table[abs_hash].status != DELETE) {
            table[abs_hash].status = DELETE;
            return true;
        } else if (table[abs_hash].status == EMPTY)
            return false;
        abs_hash = (abs_hash + i + 1) % table.size();
    }
    return false;
}

template<class T, class H>
void HashTable<T, H>::growTable() {
    std::vector<HashTableCell> new_table(table.size() * 2);
    for (auto &i: new_table)
        i = HashTableCell();

    keys_count = 0;
    for (int i = 0; i < table.size(); ++i) {
        if (table[i].status == DATA) {
            keys_count++;
            long long unsigned int abs_hash = table[i].hash % new_table.size();
            for (int j = 0; j < table.size(); ++j) {
                if (new_table[abs_hash].status == EMPTY) {
                    new_table[abs_hash] = HashTableCell(table[i].data, table[i].hash);
                    break;
                } else
                    abs_hash = (abs_hash + j + 1) % new_table.size();
            }
        }
    }
    table = std::move(new_table);
}

int main() {
    StringHasher hasher;
    HashTable<std::string, StringHasher> table(hasher);
    char operation;
    std::string data;

    while (std::cin >> operation >> data) {
        switch (operation) {
            case '+': {
                std::cout << (table.add(data) ? "OK" : "FAIL") << std::endl;
                break;
            }
            case '-': {
                std::cout << (table.del(data) ? "OK" : "FAIL") << std::endl;
                break;
            }
            case '?': {
                std::cout << (table.has(data) ? "OK" : "FAIL") << std::endl;
                break;
            }
            default:
                assert(false);
        }
    }

    return 0;
}