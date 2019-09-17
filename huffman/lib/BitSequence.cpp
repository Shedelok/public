#include <cstdio>
#include "BitSequence.h"

BitSequence::BitSequence() : data(0), bits(0), buffer(0) {
}

void BitSequence::add(bool b) {
    if (bits == 8) {
        freeBuffer();
    }
    buffer = (buffer << 1u) | b;
    bits++;
}

void BitSequence::add(unsigned char c) {
    if (bits != 0) {
        freeBuffer();
    }
    data.push_back(c);
}

void BitSequence::popBit() {
    if (bits == 0) {
        buffer = data.back();
        data.pop_back();
        bits = 8;
    }
    buffer >>= 1;
    bits--;
}


unsigned char *BitSequence::toCharArray() {
    return data.data();
}

void BitSequence::clearFullBlocks() {
    data.clear();
}

size_t BitSequence::size() {
    return fullBlocks() * 8 + bits;
}

size_t BitSequence::fullBlocks() {
    return data.size();
}

bool BitSequence::operator[](size_t ind) {
    if (ind / 8 < data.size()) {
        return data[ind / 8] & (1 << (7 - (ind % 8)));
    }
    return buffer & (1 << (bits - 1 - (ind % 8)));
}

void BitSequence::freeBuffer() {
    buffer <<= 8 - bits;
    data.push_back(buffer);
    buffer = 0;
    bits = 0;
}
