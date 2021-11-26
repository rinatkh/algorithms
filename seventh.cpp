#include <cassert>

#include <iostream>
#include <sstream>

int get_byte(long long number, size_t n) {
    return number >> (8 * n) & 0xFF;
}

void sort(long long *array, size_t n) {
    size_t amount_bytes = sizeof(long long);

    for (size_t byte = 0; byte < amount_bytes; byte++) {
        size_t cnt[256];

        for (size_t &c: cnt) {
            c = 0;
        }
        for (size_t i = 0; i < n; i++) {
            cnt[get_byte(array[i], byte)]++;
        }
        for (size_t i = 1; i < 256; i++) {
            cnt[i] += cnt[i - 1];
        }

        auto *temp_array = new long long[n];
        for (size_t k = n - 1;; k--) {
            temp_array[--cnt[get_byte(array[k], byte)]] = array[k];
            if (k == 0) {
                break;
            }
        }

        for (size_t i = 0; i < n; i++) {
            array[i] = temp_array[i];
        }
        delete[] temp_array;
    }
}

int run(std::istream &input, std::ostream &output) {
    size_t number = 0;
    input >> number;

    long long *array = new long long[number];
    for (size_t i = 0; i < number; i++)
        input >> array[i];

    sort(array, number);

    for (size_t i = 0; i < number; i++) {
        if (i == number - 1) {
            output << array[i];
            break;
        }
        output << array[i] << " ";
    }

    delete[] array;
    return 0;
}

void test() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "3\n"
                 "4 1000000 7\n";
        run(input, output);
        assert(output.str() == "4 7 1000000");
    }
}

int main() {
    run(std::cin, std::cout);
//    test();
    return 0;
}