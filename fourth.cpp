#include <cassert>

#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>


template<class T>
class Heap {
public:
    explicit Heap(bool (*compareFunc)(const T &a, const T &b));

    Heap(const T *array, size_t size,
         bool (*compareFunc)(const T &a, const T &b));

    Heap(const Heap &) = delete;

    Heap &operator=(const Heap &a) = delete;

    ~Heap();

    void insert(const T &value);

    T extractMax();

    const T &peekMax() const {

        assert(!isEmpty());
        return buffer[0];
    };

    size_t getSize() const {
        return size;
    }

    bool isEmpty() const { return size == 0; }

private:
    void buildHeap();

    void shiftDown(size_t index);

    void shiftUp(size_t index);

    void swap(size_t a, size_t b);

    bool (*compare)(const T &a, const T &b);

    void grow();


    size_t size = 0;
    size_t bufferSize = 0;
    T *buffer = nullptr;
};

template<class T>
void Heap<T>::shiftDown(size_t index) {
    size_t left = 2 * index + 1;
    size_t right = 2 * index + 2;
    size_t biggest = index;
    if (right < size && compare(buffer[biggest], buffer[right])) {
        biggest = right;
    }
    if (left < size && compare(buffer[biggest], buffer[left])) {
        biggest = left;
    }
    if (biggest != index) {
        swap(index, biggest);
        shiftDown(biggest);
    }
}

template<class T>
void Heap<T>::buildHeap() {
    size_t parent = size / 2 - 1;

    for (size_t i = 0; i <= parent; ++i) {
        shiftDown(parent - i);
    }
}

template<class T>
void Heap<T>::swap(size_t a, size_t b) {
    assert(a < size && b < size);

    T temp = buffer[a];
    buffer[a] = buffer[b];
    buffer[b] = temp;
}

template<class T>
void Heap<T>::shiftUp(size_t index) {
    while (index > 0) {
        size_t parent = (index - 1) / 2;
        if (!compare(buffer[parent], buffer[index])) {
            return;
        }
        swap(index, parent);
        index = parent;
    }
}

template<class T>
void Heap<T>::insert(const T &value) {
    if (size >= bufferSize) {
        grow();
    }
    buffer[size] = value;
    ++size;
    shiftUp(size - 1);
}

template<class T>
void Heap<T>::grow() {

    int newCapacity = 4;
    if (bufferSize != 0) {
        newCapacity = bufferSize * 2;
    }
    T *newBuffer = new T[newCapacity];
    for (int i = 0; i < size; ++i) {
        newBuffer[i] = buffer[i];
    }
    delete[] buffer;
    bufferSize = newCapacity;
    buffer = newBuffer;
}

template<class T>
T Heap<T>::extractMax() {
    assert(!isEmpty());
    T result = buffer[0];
    buffer[0] = buffer[size - 1];
    --size;
    if (!isEmpty()) {
        shiftDown(0);
    }
    return result;
}

template<class T>
Heap<T>::~Heap() {
    delete[] buffer;
}

template<class T>
Heap<T>::Heap(bool (*compareFunc)(const T &, const T &))
        : size(0), bufferSize(0), buffer(nullptr), compare(compareFunc) {}

template<class T>
Heap<T>::Heap(const T *array, size_t size,
              bool (*compareFunc)(const T &, const T &))
        : size(size), bufferSize(size), buffer(nullptr), compare(compareFunc) {
    buffer = new T[size];
    for (size_t i = 0; i < size; ++i) {
        buffer[i] = array[i];
    }
    if (size > 1) {
        buildHeap();
    }
}

struct Element {
    Element() = default;

    Element(int value_, int Index_, int ArrayIndex_) : value(value_),
                                                       Index(Index_),
                                                       ArrayIndex(ArrayIndex_) {};
    int value;
    int Index;
    int ArrayIndex;
};

bool compare(const Element &a, const Element &b) {
    return a.value < b.value;
}

void getArray(const std::vector<std::vector<int>> &data, int numberOfArrays, int max_size,
              std::vector<int> &out_data) {


    Heap<Element> heap(compare);
    for (int i = 0; i < numberOfArrays; i++) {
        if (data[i].size() > 0) {
            Element elem = Element(data[i][0], 0, i);
            heap.insert(elem);
        }
    }
    while (!heap.isEmpty()) {
        Element elem = heap.extractMax();
        out_data.push_back(elem.value);
        if (elem.Index + 1 < data[elem.ArrayIndex].size()) {
            elem.Index += 1;
            elem.value = data[elem.ArrayIndex][elem.Index];
            heap.insert(elem);
        }
    }
}


void run(std::istream &input, std::ostream &output) {
    int n = 0;
    input >> n;

    int sizeArray = 0;

    int value = 0;
    std::vector<std::vector<int>> data;
    data.resize(n);

    int max_size = 0;
    for (int i = 0; i < n; i++) {
        input >> sizeArray;
        for (int j = 0; j < sizeArray; j++) {
            input >> value;
            data[i].push_back(value);
        }
        if (max_size < sizeArray) {
            max_size = sizeArray;
        }
    }

    for (int i = 0; i < n; ++i) {
        std::reverse(data[i].begin(), data[i].end());
    }

    std::vector<int> out_data;
    getArray(data, n, max_size, out_data);
    for (int i = 0; i < out_data.size(); ++i) {
        if (i == out_data.size() - 1) {
            output << out_data[out_data.size() - (i + 1)];
            break;
        }
        output << out_data[out_data.size() - (i + 1)] << " ";
    }
}

void test() {

    {
        std::stringstream input;
        std::stringstream output;
        input << "3\n"
                 "1\n"
                 "6\n"
                 "2\n"
                 "50 90\n"
                 "3\n"
                 "1 10 20\n";
        run(input, output);
        assert(output.str() == "1 6 10 20 50 90");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "2\n"
                 "2\n"
                 "6 6\n"
                 "2\n"
                 "10 10\n";
        run(input, output);
        assert(output.str() == "6 6 10 10");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "0\n";
        run(input, output);
        assert(output.str() == "");
    }

}

int main() {
    run(std::cin, std::cout);
//    test();
    return 0;
}