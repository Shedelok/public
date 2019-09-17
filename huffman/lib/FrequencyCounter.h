#ifndef HUFFMAN_FREQUENCYCOUNTER_H
#define HUFFMAN_FREQUENCYCOUNTER_H

#include <vector>
#include <memory.h>
#include "Node.h"

struct FrequencyCounter {
    FrequencyCounter();

    ~FrequencyCounter();

    void import(unsigned char const *, size_t);

    size_t &operator[](size_t);

    size_t operator[](size_t) const;

private:
    size_t *frequency;
};

#endif //HUFFMAN_FREQUENCYCOUNTER_H
