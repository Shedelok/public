#include "FrequencyCounter.h"

FrequencyCounter::FrequencyCounter() {
    frequency = new size_t[256]{};
}

FrequencyCounter::~FrequencyCounter() {
    delete[] frequency;
}


void FrequencyCounter::import(unsigned char const *data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        frequency[data[i]]++;
    }
}

size_t &FrequencyCounter::operator[](size_t ind) {
    return frequency[ind];
}

size_t FrequencyCounter::operator[](size_t ind) const {
    return frequency[ind];
}
