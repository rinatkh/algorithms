#include <iostream>

void getRangePositions(int &left, int &rigth, const int *array, const int element,
                  const int arraySize) {
    int powerIndex = 1;
    int prevPowerIndex = 0;
    do {
        if (array[powerIndex] >= element) {
            break;
        }
        prevPowerIndex = powerIndex;
        powerIndex = 2 * powerIndex;
        if (powerIndex > arraySize) {
            powerIndex = arraySize;
        }
    } while (powerIndex < arraySize);
    rigth = powerIndex;
    left = prevPowerIndex;
}

int getMinIndex(int element, const int *array, const int arraySize) {
    int leftPosition = 0, rigthPosition = 0;
    getRangePositions(leftPosition, rigthPosition, array, element, arraySize);
    while (leftPosition < rigthPosition) {
        int mid = (leftPosition + rigthPosition) / 2;
        if (array[mid] < element) {
            leftPosition = mid + 1;
        } else {
            rigthPosition = mid;
        }
    }
    return (array[leftPosition] < element) ? arraySize : leftPosition;
}

int main() {
    int firstArraySize = 0, secondArraySize = 0;
    std::cin >> firstArraySize >> secondArraySize;
    int *firstArray = new int[firstArraySize];
    int *secondArray = new int[secondArraySize];

    for (int i = 0; i != firstArraySize; i++) {
        std::cin >> firstArray[i];
    }
    for (int i = 0; i != secondArraySize; i++) {
        std::cin >> secondArray[i];
    }

    for (int i = 0; i != secondArraySize; i++) {
        std::cout << getMinIndex(secondArray[i], firstArray, firstArraySize)
                  << ((i != (secondArraySize - 1)) ? " " : "");
    }
    delete[] firstArray;
    delete[] secondArray;
}

