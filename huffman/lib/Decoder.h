#ifndef HUFFMAN_DECODER_H
#define HUFFMAN_DECODER_H

#include <vector>
#include <stack>
#include <cstdint>
#include "Node.h"

struct Decoder {
    Decoder();

    ~Decoder();

    void import(unsigned char const *, size_t, std::vector<unsigned char> &);

    void end(std::vector<unsigned char> &);

private:
    Node *root;
    Node *decodingNode;
    std::stack<Node *> stack;
    std::stack<Node *> leafs;
    uint8_t bitsInLastChar;
    unsigned char lastChar;
    bool started;

    void workWithChar(unsigned char, std::vector<unsigned char> &, bool);

    void workWithBit(bool, std::vector<unsigned char> &);

    void buildTree(bool);
};

#endif //HUFFMAN_DECODER_H
