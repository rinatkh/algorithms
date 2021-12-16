#include <sstream>
#include <iostream>
#include <cassert>
#include <queue>
#include <vector>
#include <stack>

template<class T>
struct compare_default {
    bool operator()(const T &a, const T &b) {
        return a < b;
    }
};

template<class T, class Compare = compare_default<T>>
class BinaryTree {
public:
    explicit BinaryTree(const Compare &compare_ = compare_default<T>()) : root(nullptr),
                                                                          compare(compare_) {}

    ~BinaryTree();

    bool add(const int &data);

    void post_order(std::vector<int> &array_post_order);

private:
    struct ThreeNode {
        T data;
        ThreeNode *left, *right;

        explicit ThreeNode(const int &data) : data(data), left(nullptr), right(nullptr) {}
    };

    Compare compare;
    ThreeNode *root;
};

template<class T, class Compare>
BinaryTree<T, Compare>::~BinaryTree() {
    std::queue<ThreeNode *> tmp;
    if (root)
        tmp.push(root);

    while (!tmp.empty()) {
        ThreeNode *current = tmp.front();
        if (current->left)
            tmp.push(current->left);
        if (current->right)
            tmp.push(current->right);

        delete current;
        tmp.pop();
    }
}

template<class T, class Compare>
bool BinaryTree<T, Compare>::add(const int &data) {
    auto *newNode = new ThreeNode(data);
    if (root == nullptr) {
        root = newNode;
        return true;
    }

    ThreeNode *nextNode = root;

    while (true) {
        if (compare(data, nextNode->data)) {
            if (nextNode->left != nullptr)
                nextNode = nextNode->left;
            else {
                nextNode->left = newNode;
                return true;
            }
        } else {
            if (nextNode->right != nullptr)
                nextNode = nextNode->right;
            else {
                nextNode->right = newNode;
                return true;
            }
        }
    }
}

template<class T, class Compare>
void BinaryTree<T, Compare>::post_order(std::vector<int> &array_post_order) {
    if (root == nullptr) {
        return;
    }
    ThreeNode *node = root;
    ThreeNode *head = root;

    std::stack<ThreeNode *> stack;
    stack.push(node);

    while (!stack.empty()) {
        node = stack.top();

        bool is_head = (node->right == head || node->left == head);
        bool is_leaf = (node->left == nullptr && node->right == nullptr);

        if (is_head || is_leaf) {
            stack.pop();
            array_post_order.push_back(node->data);
            head = node;
        } else {
            if (node->right != nullptr) {
                stack.push(node->right);
            }
            if (node->left != nullptr) {
                stack.push(node->left);
            }
        }
    }
}


int run(std::istream &input, std::ostream &output) {

    size_t number;
    input >> number;

    int buf;

    BinaryTree<int> tree;
    for (size_t i = 0; i < number; ++i) {
        input >> buf;
        tree.add(buf);
    }

    std::vector<int> array_post_order;
    tree.post_order(array_post_order);


    for (size_t i = 0; i < number; ++i) {
        if (i == number - 1) {
            output << array_post_order[i];
        } else {
            output << array_post_order[i] << " ";
        }
    }

    return 0;
}

void test() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "10\n7\n2\n10\n8\n5\n3\n6\n4\n1\n9\n";
        run(input, output);
        assert(output.str() == "1 4 3 6 5 2 9 8 10 7");
    }
}

int main() {
    run(std::cin, std::cout);
//    test();
    return 0;
}