#include <iostream>

unsigned int addOne(unsigned int number) {
    unsigned int result = 0, inter = 0;
    result = number ^ 1;
    inter = (number & 1) << 1;
    while (inter) {
        unsigned int tmp = result;
        result = result ^ inter;
        inter = (tmp & inter) << 1;
    }
    return result;
}

unsigned int amoutEvenOnes(unsigned int number) {
    unsigned int counter = 0;
    while (number > 0) {
        if ((number & 1) == 1) {
            counter = addOne(counter);
        }
        number = number >> 2;
    }
    return counter;
}


int main() {
    unsigned int number;
    std::cin >> number;
    std::cout << amoutEvenOnes(number);
    return 0;
}
