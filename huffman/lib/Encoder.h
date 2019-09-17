#ifndef HUFFMAN_ENCODER_H
#define HUFFMAN_ENCODER_H

#include "Tree.h"

struct Encoder {
    Encoder(Tree const &);

    ~Encoder();

    void import(unsigned char const *, size_t, std::vector<unsigned char> &);

    uint8_t bitsInLastChar();

    void end(std::vector<unsigned char> &c);

private:
    Tree const *tree;
    BitSequence *codes;
    BitSequence buffer;

    void writeBuffer(std::vector<unsigned char> &);

    void calcCodes(Node *, BitSequence &);
};

#endif //HUFFMAN_ENCODER_H
