#include <cassert>
#include <iostream>
#include <sstream>

template<class T>
struct compare_default {
    bool operator()(const T &a, const T &b) {
        return a <= b;
    }
};

template<class T, class Compare>
int partition(T *array, int begin, int end, Compare compare) {
    int pivot_index = rand() % (end - begin + 1) + begin;
    int pivot = array[pivot_index];
    std::swap(array[pivot_index], array[end]);
    int i = begin, j = begin;
    while (i <= end && compare(array[i], pivot)) {
        i++;
    }
    if (i >= end) {
        return end;
    }
    j = i + 1;

    while (j < end) {
        if (compare(array[j], pivot) && j < end) {
            std::swap(array[i++], array[j]);
        }
        j++;
    }
    std::swap(array[i], array[end]);
    return i;
}

template<class T, class Compare = compare_default<T>>
int order_statistic(T *array, int size_array, int index,
                    const Compare &compare = compare_default<T>()) {
    int pivot = -1;
    int begin = 0, end = size_array - 1;
    while (pivot != index) {
        pivot = partition(array, begin, end, compare);
        if (pivot < index) {
            begin = pivot + 1;
        } else {
            end = pivot - 1;
        }
    }
    return array[index];
}

void run(std::istream &input, std::ostream &output) {
    int n = 0;
    input >> n;
    int *array = new int[n];
    for (int i = 0; i < n; ++i) {
        input >> array[i];
    }
    output << order_statistic(array, n, n / 10) << std::endl;
    output << order_statistic(array, n, n / 2) << std::endl;
    output << order_statistic(array, n, n * 9 / 10) << std::endl;
    delete[] array;
}

void test() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "10\n"
                 "1 2 3 4 5 6 7 8 9 10\n";
        run(input, output);
        assert(output.str() == "2\n"
                               "6\n"
                               "10\n");
    }
}

int main() {
    run(std::cin, std::cout);
//    test();
    return 0;
}