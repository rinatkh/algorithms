#include <vector>
#include <cassert>
#include <iostream>
#include <queue>
#include <cmath>


template<class T>
struct compare_default {
    bool operator()(const T &a, const T &b) {
        return a < b;
    }
};

template<class T, class Compare = compare_default<T>>
class BTree {
public:
    explicit BTree(size_t _t, const Compare &compare_ = compare_default<T>()) : h(0),
                                                                                root(nullptr),
                                                                                t(_t),
                                                                                compare(compare_) {
        assert(t >= 2);
    }

    ~BTree();

    void insert(const T &key);

    void print_tree();

    size_t h;

private:

    struct Node {
        bool is_leaf;
        std::vector<T> keys;
        std::vector<Node *> children;

        explicit Node(bool _is_leaf) : is_leaf(_is_leaf) {}
    };

    Node *root;
    size_t t;
    Compare compare;

    void insert_non_full(Node *node, const T &key);

    bool is_full_node(Node *node) const;

    void split_child(Node *x, int idx);

    void fill_array_for_print(BTree::Node *node,
                              size_t indent,
                              std::vector<std::pair<int, std::pair<size_t, size_t>>> &array);
};

template<class T, class Compare>
BTree<T, Compare>::~BTree() {
    if (root != nullptr) {
        std::queue<Node *> tmp;
        Node *node = root;
        tmp.push(node);
        while (!tmp.empty()) {
            Node *current = tmp.front();
            for (auto &i: current->children) {
                tmp.push(i);
            }
            delete current;
            tmp.pop();
        }
    }
}

template<class T, class Compare>
void BTree<T, Compare>::insert(const T &key) {
    if (!root) {
        root = new Node(true);
    }
    if (is_full_node(root)) {
        Node *new_node = new Node(false);
        new_node->children.push_back(root);
        root = new_node;
        split_child(root, 0);
    }
    insert_non_full(root, key);
}


template<class T, class Compare>
void BTree<T, Compare>::insert_non_full(BTree::Node *node, const T &key) {
    int pos = node->keys.size() - 1;
    if (node->is_leaf) {
        node->keys.resize(node->keys.size() + 1);
        while (pos >= 0 && compare(key, node->keys[pos])) {
            node->keys[pos + 1] = node->keys[pos];
            pos--;
        }
        node->keys[pos + 1] = key;
    } else {
        while (pos >= 0 && compare(key, node->keys[pos])) {
            pos--;
        }
        if (is_full_node(node->children[pos + 1])) {
            split_child(node, pos + 1);
            if (compare(node->keys[pos + 1], key)) {
                pos++;
            }
        }
        insert_non_full(node->children[pos + 1], key);
    }
}

template<class T, class Compare>
bool BTree<T, Compare>::is_full_node(BTree::Node *node) const {
    return node->keys.size() == 2 * t - 1;
}

template<class T, class Compare>
void BTree<T, Compare>::split_child(BTree::Node *x, int idx) {
    Node *y = x->children[idx];
    Node *z = new Node(y->is_leaf);

    for (size_t j = 0; j < t - 1; j++) {
        z->keys.push_back(y->keys[j + t]);
    }

    if (!y->is_leaf) {
        for (size_t j = 0; j < t; j++)
            z->children.push_back(y->children[j + t]);
    }
    x->keys.resize(x->keys.size() + 1);
    x->children.resize(x->children.size() + 1);

    for (size_t j = x->keys.size() - 1; j > idx; j--) {
        x->keys[j] = x->keys[j - 1];
    }
    x->keys[idx] = y->keys[t - 1];
    y->keys.resize(t - 1);

    if (!y->is_leaf) {
        y->children.resize(t);
    }
    for (size_t j = x->children.size() - 1; j > idx + 1; j--) {
        x->children[j] = x->children[j - 1];
    }
    x->children[idx + 1] = z;
}

template<class T, class Compare>
void BTree<T, Compare>::fill_array_for_print(BTree::Node *node,
                                             size_t indent,
                                             std::vector<std::pair<int, std::pair<size_t, size_t>>> &array) {
    if (node != nullptr) {
        for (auto &i: node->keys) {
            array.push_back({i, {indent, node->keys.size()}});
        }
        for (auto child: node->children) {
            h = indent + 1;
            fill_array_for_print(child, indent + 1, array);
        }
    }
}

template<class T, class Compare>
void BTree<T, Compare>::print_tree() {
    if (!root) {
        return;
    }
    std::vector<std::pair<int, std::pair<size_t, size_t>>> array;
    fill_array_for_print(root, 0, array);
    for (auto &i: array) {
        std::cout << i.first << " " << i.second.first << " " << i.second.second << "\n";
    }
    size_t count = 1;
    for (size_t j = 0; j <= h; ++j) {
        for (auto &i: array) {
            if ((i.second.first == j) && (count++ < (i.second.second - 1))) {
                std::cout << i.first << " ";
            } else if ((i.second.first == j) && (count++ == (i.second.second - 1))) {
                std::cout << i.first << std::endl;
            }
        }
        count = 1;
    }
}


int main() {
    int t = 0;
    std::cin >> t;
    BTree<int> tree(t);
    int key = 0;
    size_t number = 0;
    while (std::cin >> key) {
        tree.insert(key);
        number++;
    }
    tree.print_tree();
    return 0;
}
