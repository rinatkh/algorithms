#include <iostream>
#include <cassert>

class Stack {
public:
    Stack() : bufferSize(100), buffer(new int[100]), currentSize(0) {};

    Stack &operator=(const Stack &) = delete;

    Stack(const Stack &) = delete;

    ~Stack() { delete[] buffer; }

    bool isEmpty() const;

    void push(int element);

    int pop();

    int getSize() const;

private:
    int *buffer;
    int currentSize;
    int bufferSize;

    void grow();
};

void Stack::grow() {
    int newBufferSize = 2 * bufferSize;
    int *newBuffer = new int[newBufferSize];
    for (int i = 0; i != bufferSize; i++) {
        newBuffer[i] = buffer[i];
    }
    delete[] buffer;
    buffer = newBuffer;
    bufferSize = newBufferSize;
}

int Stack::getSize() const {
    return currentSize;
}

bool Stack::isEmpty() const {
    if (currentSize == 0) {
        return true;
    }
    return false;
}

int Stack::pop() {
    assert(!isEmpty());
    return buffer[--currentSize];
}

void Stack::push(int element) {
    if ((currentSize + 1) == bufferSize) {
        grow();
    }
    buffer[currentSize++] = element;
}


class Queue {
public:
    Queue() : input(new Stack()), output(new Stack()) {};

    ~Queue();

    bool isEmpty() const;

    void pushBack(const int value) { input->push(value); }

    int popFront();


private:
    Stack *input;
    Stack *output;
};

Queue::~Queue() {
    delete input;
    delete output;
}

bool Queue::isEmpty() const {
    if (input->isEmpty() && output->isEmpty()) {
        return true;
    }
    return false;
}

int Queue::popFront() {
    assert(!isEmpty());
    if (!output->isEmpty()) {
        return output->pop();
    } else {
        int inputSize = input->getSize();
        for (int i = 0; i != inputSize; i++) {
            output->push(input->pop());
        }
        return output->pop();
    }
}


int main() {
    Queue queue;
    bool result = true;
    int count = 0;
    std::cin >> count;
    for (int i = 0; i != count; i++) {
        int command = 0, value = 0;
        std::cin >> command >> value;
        switch (command) {
            case 2: {
                if (queue.isEmpty()) {
                    result = (result && value == -1);
                } else {
                    result = (result && queue.popFront() == value);
                }
                break;
            }
            case 3: {
                queue.pushBack(value);
                break;
            }
            default: {
                assert(false);
            }
        }
    }
    std::cout << (result ? "YES" : "NO");
    return 0;
}

