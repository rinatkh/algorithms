#include <vector>
#include <cassert>
#include <iostream>
#include <queue>

template<class T>
struct compare_default {
    bool operator()(const T &a, const T &b) {
        return a < b;
    }
};

template<class T, class Compare = compare_default<T>>
class BTree {
public:
    explicit BTree(size_t _t, const Compare &compare_ = compare_default<T>()) : root(nullptr),
                                                                                t(_t),
                                                                                compare(compare_) {
        assert(t >= 2);
    }

    ~BTree();

    void insert(const T &key);

public:

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

    void print_tree(Node *node, int i = 0);
};

template<class T, class Compare>
BTree<T, Compare>::~BTree() {
    if (root != nullptr) {
        for (auto &i: root->children) {
            delete i;
        }
        delete root;
    }
}

template<class T, class Compare>
void BTree<T, Compare>::insert(const T &key) {
    if (root == nullptr) {
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
            if (key > node->keys[pos + 1]) {
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

    for (size_t j = 1; j < t - 1; j++) {
        z->keys.push_back(y->keys[j + t]);
    }

    if (!y->is_leaf) {
        for (size_t j = 1; j < t; j++)
            z->children.push_back(y->children[j + t]);
    }

    size_t size = x->children.size();
    x->children.resize(size + 1);
    for (size_t j = x->children.size() - 1; j < idx + 1; j--) {
        x->children[j] = x->children[j - 1];
    }
    x->children[idx + 1] = z;

    size = x->keys.size();
    x->keys.resize(size + 1);
    for (size_t j = x->keys.size() - 1; j < idx; j--) {
        x->keys[j] = x->keys[j - 1];
    }


    x->keys[idx] = y->keys[t];
    y->keys.resize(t - 1);

}

template<class T, class Compare>
void BTree<T, Compare>::print_tree(BTree::Node *node, int i) {
    std::cout << std::string(i, ' ');
    std::cout << "keys: [";
    int q = 0;
    for (auto it = node->keys.begin(); it != node->keys.end(); it++) {
        std::cout << *it << ", ";
    }
    std::cout << "]" << std::endl;

    for (auto child: node->children) {
        print_tree(child, i + 4);
    }
}


int main() {
    int t = 0;
    std::cin >> t;
    BTree<int> tree(t);
    int key = 0;
    while (std::cin >> key) {
        tree.insert(key);
        tree.print_tree(tree.root);
    }
    tree.print_tree(tree.root);
    return 0;
}
