#ifndef HUFFMAN_BITSEQUENCE_H
#define HUFFMAN_BITSEQUENCE_H

#include <vector>
#include <cstdint>

struct BitSequence {
    BitSequence();

    BitSequence(BitSequence const &) = default;

    void add(bool);

    void add(unsigned char);

    void popBit();

    unsigned char *toCharArray();

    void clearFullBlocks();

    size_t size();

    size_t fullBlocks();

    BitSequence &operator=(BitSequence const &rhs) = default;

    bool operator[](size_t);

private:
    std::vector<unsigned char> data;
    uint8_t bits;
    unsigned char buffer;

    void freeBuffer();
};


#endif //HUFFMAN_BITSEQUENCE_H
