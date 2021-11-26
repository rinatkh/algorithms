#include <cassert>

#include <iostream>
#include <sstream>

template<class T>
struct compare_default {
    bool operator()(const T &a, const T &b) {
        return a < b;
    }
};

template<class T, class Compare>
void merge(T *a, size_t begin, size_t middle, size_t end, Compare compare) {
    size_t p = begin, q = middle + 1, k = 0;
    T *array_buffer = new T[end - begin + 1];

    for (size_t i = begin; i <= end; ++i) {
        if (compare(middle, p)) {
            array_buffer[k++] = a[q++];
        } else if (compare(end, q)) {
            array_buffer[k++] = a[p++];
        } else if (compare(a[p], a[q])) {
            array_buffer[k++] = a[p++];
        } else {
            array_buffer[k++] = a[q++];
        }
    }

    for (size_t i = 0; i < k; ++i) {
        a[begin++] = array_buffer[i];
    }

    delete[] array_buffer;
}

template<class T, class Compare = compare_default<T>>
void merge_sort(T *array, size_t begin, size_t end, const Compare &compare = compare_default<T>()) {
    if (begin < end) {
        merge_sort(array, begin, (begin + end) / 2);
        merge_sort(array, (begin + end) / 2 + 1, end);
        merge(array, begin, (begin + end) / 2, end, compare);
    }
}

int
count_seg_of_one(int *array_segments_begin, int *array_segments_end, int number, int max_value) {
    merge_sort(array_segments_begin, 0, number - 1);
    merge_sort(array_segments_end, 0, number - 1);
    int count = 0, layer = 0;
    int i = 0, j = 0;

    for (size_t k = 0; k <= max_value; ++k) {
        if (layer == 1) {
            count++;
        }
        if (array_segments_begin[i] == k) {
            layer++;
            i++;
            while (array_segments_begin[i - 1] == array_segments_begin[i]) {
                layer++;
                i++;
            }
        }
        if (array_segments_end[j] == k) {
            layer--;
            j++;
            while (array_segments_end[j - 1] == array_segments_end[j]) {
                layer--;
                j++;
            }
        }
    }
    return count;
}

void run(std::istream &input, std::ostream &output) {
    size_t number = 0;
    input >> number;

    int *array_segments_start = new int[number];
    int *array_segments_end = new int[number];

    size_t max_value = 0;
    for (size_t i = 0; i < number; ++i) {
        input >> array_segments_start[i];
        input >> array_segments_end[i];
        if (array_segments_end[i] > max_value) {
            max_value = array_segments_end[i];
        }
    }

    output << count_seg_of_one(array_segments_start, array_segments_end, number, max_value);

    delete[] array_segments_start;
    delete[] array_segments_end;

}

void test() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "3\n"
                 "1 4\n"
                 "7 8\n"
                 "2 5\n";
        run(input, output);
        assert(output.str() == "3");
    }
}

int main() {
    run(std::cin, std::cout);
//    test();
    return 0;
}