#include <iostream>
#include <cassert>
#include <queue>

template<class T>
struct compare_default {
    bool operator()(const T &a, const T &b) {
        return a > b;
    }
};

template<class T, class Compare = compare_default<T>>
class BinaryTree {
public:

    explicit BinaryTree(const Compare &compare = Compare());

    virtual ~BinaryTree();

    bool search(const T &key);

protected:

    struct TreeNode {
        T key;
        TreeNode *left;
        TreeNode *right;
        char height;
        int weight;

        explicit TreeNode(const T &key) : key(key),
                                          left(nullptr),
                                          right(nullptr),
                                          height(1),
                                          weight(1) {}
    };

    TreeNode *root;
    int size;
    Compare cmp;

    TreeNode *find_min(TreeNode *_root);

private:

    std::pair<bool, TreeNode *> find(const T &key);

};

template<class T, class Compare>
BinaryTree<T, Compare>::BinaryTree(const Compare &compare) : root(nullptr),
                                                             size(0),
                                                             cmp(compare) {}

template<class T, class Cmp>
BinaryTree<T, Cmp>::~BinaryTree() {
    std::queue<TreeNode *> queue;

    if (root != nullptr) {
        queue.push(root);
    }

    while (!queue.empty()) {
        if (queue.front()->left != nullptr) {
            TreeNode *tmp = queue.front()->left;
            queue.front()->left = nullptr;
            queue.push(tmp);
        } else if (queue.front()->right != nullptr) {
            TreeNode *tmp = queue.front()->right;
            queue.front()->right = nullptr;
            queue.push(tmp);
        } else {
            delete queue.front();
            queue.pop();
        }
    }
}

template<class T, class Compare>
bool BinaryTree<T, Compare>::search(const T &key) {
    return find(key).first;
}

template<class T, class Compare>
class BinaryTree<T, Compare>::TreeNode *BinaryTree<T, Compare>::find_min(TreeNode *_root) {
    if (_root == nullptr) {
        return nullptr;
    }
    while (_root->left != nullptr) {
        _root = _root->left;
    }
    return _root;
}


template<class T, class Compare>
std::pair<bool, class BinaryTree<T, Compare>::TreeNode *>
BinaryTree<T, Compare>::find(const T &key) {
    TreeNode *current = root;

    while (true) {
        if (current == nullptr) {
            return {false, nullptr};
        } else if (!(cmp(current->key, key)) && !(cmp(key, current->key))) {
            return {true, current};
        } else if (cmp(key, current->key)) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
}

template<class T, class Compare = compare_default<T>>
class AVLTree : public BinaryTree<T, Compare> {
public:

    explicit AVLTree(const Compare &compare = Compare());

    ~AVLTree() = default;

    void insert(const T &key);

    bool erase(const T &key);

    int position(const T &key);

    std::pair<bool, T> statistics(int index);

private:

    int weight(class BinaryTree<T, Compare>::TreeNode *node);

    char height(class BinaryTree<T, Compare>::TreeNode *node);

    char bfactor(class BinaryTree<T, Compare>::TreeNode *node);

    int position_trail(class BinaryTree<T, Compare>::TreeNode *node, const T &key);

    T statistics_trail(class BinaryTree<T, Compare>::TreeNode *node, int index);

    class BinaryTree<T, Compare>::TreeNode *avl_insert(class BinaryTree<T, Compare>::TreeNode *root,
                                                       const T &key);

    class BinaryTree<T, Compare>::TreeNode *avl_erase(class BinaryTree<T, Compare>::TreeNode *root,
                                                      const T &key);

    class BinaryTree<T, Compare>::TreeNode *
    remove_min(class BinaryTree<T, Compare>::TreeNode *root);

    class BinaryTree<T, Compare>::TreeNode *
    balance(class BinaryTree<T, Compare>::TreeNode *balancing);

    void lest_rotate(class BinaryTree<T, Compare>::TreeNode *avl_rotate);

    void rigth_rotate(class BinaryTree<T, Compare>::TreeNode *avl_rotate);

};

template<class T, class Compare>
AVLTree<T, Compare>::AVLTree(const Compare &compare)
        :BinaryTree<T, Compare>::BinaryTree(compare) {}

template<class T, class Compare>
void AVLTree<T, Compare>::insert(const T &key) {
    BinaryTree<T, Compare>::root = avl_insert(BinaryTree<T, Compare>::root, key);
    ++BinaryTree<T, Compare>::size;
}

template<class T, class Compare>
bool AVLTree<T, Compare>::erase(const T &key) {

    if (BinaryTree<T, Compare>::search(key)) {
        BinaryTree<T, Compare>::root = avl_erase(BinaryTree<T, Compare>::root, key);
        --BinaryTree<T, Compare>::size;
        return true;
    } else {
        return false;
    }
}

template<class T, class Compare>
int AVLTree<T, Compare>::position(const T &key) {
    return position_trail(BinaryTree<T, Compare>::root, key);
}

template<class T, class Compare>
std::pair<bool, T> AVLTree<T, Compare>::statistics(int index) {

    if (index >= BinaryTree<T, Compare>::size) {
        return {false, 0};
    }

    return {true, statistics_trail(BinaryTree<T, Compare>::root, index)};
}

template<class T, class Compare>
inline int AVLTree<T, Compare>::weight(class BinaryTree<T, Compare>::TreeNode *node) {
    return (node) ? node->weight : 0;
}

template<class T, class Compare>
inline char AVLTree<T, Compare>::height(class BinaryTree<T, Compare>::TreeNode *node) {
    return (node) ? node->height : 0;
}

template<class T, class Compare>
inline char AVLTree<T, Compare>::bfactor(class BinaryTree<T, Compare>::TreeNode *node) {
    return height(node->left) - height(node->right);
}

template<class T, class Compare>
int AVLTree<T, Compare>::position_trail(class BinaryTree<T, Compare>::TreeNode *node,
                                        const T &key) {

    if (node == nullptr) {
        return 0;
    }

    if (BinaryTree<T, Compare>::cmp(key,
                                    node->key)) {
        return position_trail(node->left, key);
    }

    return position_trail(node->right, key) + weight(node->left) + 1;
}

template<class T, class Compare>
T AVLTree<T, Compare>::statistics_trail(class BinaryTree<T, Compare>::TreeNode *node, int index) {

    if (weight(node->left) == index) {
        return node->key;
    }

    if (index < weight(node->left)) {
        return statistics_trail(node->left, index);
    }

    return statistics_trail(node->right, index - (weight(node->left) + 1));

}

template<class T, class Compare>
class BinaryTree<T, Compare>::TreeNode *AVLTree<T, Compare>::avl_insert
        (class BinaryTree<T, Compare>::TreeNode *root, const T &key) {

    if (root == nullptr) {
        root = new class BinaryTree<T, Compare>::TreeNode(key);
    } else {
        if (BinaryTree<T, Compare>::cmp(key, root->key)) {
            root->left = avl_insert(root->left, key);
        } else {
            root->right = avl_insert(root->right, key);
        }
    }

    return balance(root);
}

template<class T, class Compare>
class BinaryTree<T, Compare>::TreeNode *AVLTree<T, Compare>::avl_erase
        (class BinaryTree<T, Compare>::TreeNode *root, const T &key) {

    if (BinaryTree<T, Compare>::cmp(root->key, key)) {
        root->right = avl_erase(root->right, key);
    } else if (BinaryTree<T, Compare>::cmp(key, root->key)) {
        root->left = avl_erase(root->left, key);
    } else {
        if (root->right == nullptr) {
            class BinaryTree<T, Compare>::TreeNode *tmp = root->left;
            delete root;
            return tmp;
        }
        class BinaryTree<T, Compare>::TreeNode *min = BinaryTree<T, Compare>::find_min(root->right);
        root->right = remove_min(root->right);
        class BinaryTree<T, Compare>::TreeNode *tmp_left = root->left;
        class BinaryTree<T, Compare>::TreeNode *tmp_right = root->right;
        delete root;
        root = min;
        min->left = tmp_left;
        min->right = tmp_right;
    }

    return balance(root);
}

template<class T, class Compare>
class BinaryTree<T, Compare>::TreeNode *AVLTree<T, Compare>::remove_min
        (class BinaryTree<T, Compare>::TreeNode *root) {

    if (root->left == nullptr) {
        return root->right;
    }

    root->left = remove_min(root->left);

    return balance(root);
}

template<class T, class Compare>
class BinaryTree<T, Compare>::TreeNode *AVLTree<T, Compare>::balance
        (class BinaryTree<T, Compare>::TreeNode *balancing) {

    class BinaryTree<T, Compare>::TreeNode *result = balancing;

    if (bfactor(balancing) == -2) {
        if (bfactor(balancing->right) == 1) {
            class BinaryTree<T, Compare>::TreeNode *tmp = balancing->right;
            balancing->right = balancing->right->left;
            rigth_rotate(tmp);
        }
        result = balancing->right;
        lest_rotate(balancing);

    } else if (bfactor(balancing) == 2) {
        if (bfactor(balancing->left) == -1) {
            class BinaryTree<T, Compare>::TreeNode *tmp = balancing->left;
            balancing->left = balancing->left->right;
            lest_rotate(tmp);
        }
        result = balancing->left;
        rigth_rotate(balancing);
    }

    result->weight = weight(result->left) + weight(result->right) + 1;

    result->height = std::max(height(result->left), height(result->right)) + 1;

    return result;
}

template<class T, class Compare>
void AVLTree<T, Compare>::lest_rotate(class BinaryTree<T, Compare>::TreeNode *avl_rotate) {

    class BinaryTree<T, Compare>::TreeNode *tmp = avl_rotate->right;
    avl_rotate->right = tmp->left;
    tmp->left = avl_rotate;

    avl_rotate->weight = weight(avl_rotate->left) + weight(avl_rotate->right) + 1;
    tmp->weight = weight(tmp->left) + weight(tmp->right) + 1;

    avl_rotate->height = std::max(height(avl_rotate->left), height(avl_rotate->right)) + 1;
    tmp->height = std::max(height(tmp->left), height(tmp->right)) + 1;
}

template<class T, class Compare>
void AVLTree<T, Compare>::rigth_rotate(class BinaryTree<T, Compare>::TreeNode *avl_rotate) {

    class BinaryTree<T, Compare>::TreeNode *tmp = avl_rotate->left;
    avl_rotate->left = tmp->right;
    tmp->right = avl_rotate;

    avl_rotate->weight = weight(avl_rotate->left) + weight(avl_rotate->right) + 1;
    tmp->weight = weight(tmp->left) + weight(tmp->right) + 1;

    avl_rotate->height = std::max(height(avl_rotate->left), height(avl_rotate->right)) + 1;
    tmp->height = std::max(height(tmp->left), height(tmp->right)) + 1;
}

int main() {
    int n;
    std::cin >> n;
    AVLTree<int> tree;

    for (int i = 0; i < n; ++i) {
        int command, value;
        std::cin >> command >> value;
        switch (command) {
            case 1:
                std::cout << tree.position(value) << std::endl;
                tree.insert(value);
                break;
            case 2:
                tree.erase(tree.statistics(value).second);
                break;
            default:
                assert(false);
        }
    }
    return 0;
}
