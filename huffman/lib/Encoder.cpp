#include <cstdio>
#include "Encoder.h"

Encoder::Encoder(Tree const &t) : tree(&t), buffer(tree->toBitSequence()) {
    codes = new BitSequence[256];
    BitSequence tmp;
    calcCodes(tree->getRoot(), tmp);
    uint8_t b = bitsInLastChar();
    buffer.add((bool) (b & 0b100u));
    buffer.add((bool) (b & 0b010u));
    buffer.add((bool) (b & 0b001u));
}

Encoder::~Encoder() {
    delete[] codes;
}

void Encoder::import(unsigned char const *data, size_t size, std::vector<unsigned char> &result) {
    for (size_t i = 0; i < size; i++) {
        unsigned char c = data[i];
        for (size_t j = 0; j < codes[c].size(); j++) {
            buffer.add(codes[c][j]);
        }
    }
    writeBuffer(result);
}

uint8_t Encoder::bitsInLastChar() {
    uint8_t result = 3;
    for (size_t i = 0; i < 256; i++) {
        if (codes[i].size() != 0) {
            result = (result + (tree->getFrequencyCounter()->operator[](i) % 8) * (codes[i].size() % 8)) % 8;
        }
    }
    return result;
}

void Encoder::end(std::vector<unsigned char> &result) {
    writeBuffer(result);
    if (buffer.size()) {
        while (buffer.fullBlocks() != 1) {
            buffer.add(false);
        }
    }
    writeBuffer(result);
}

void Encoder::writeBuffer(std::vector<unsigned char> &result) {
    unsigned char* arr = buffer.toCharArray();
    for (size_t i = 0; i < buffer.fullBlocks(); i++) {
        result.push_back(arr[i]);
    }
    buffer.clearFullBlocks();
}

void Encoder::calcCodes(Node *node, BitSequence &code) {
    if (node->isLeaf()) {
        codes[node->getChar()] = code;
        return;
    }
    code.add(false);
    calcCodes(node->leftChild, code);
    code.popBit();
    code.add(true);
    calcCodes(node->rightChild, code);
    code.popBit();
}
