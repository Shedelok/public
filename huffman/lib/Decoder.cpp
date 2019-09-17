#include <cstdio>
#include "Decoder.h"
#include "FileCorruptedException.h"

Decoder::Decoder() : bitsInLastChar(UINT8_MAX), started(false), lastChar() {
    decodingNode = root = new Node;
    stack.push(root);
}

Decoder::~Decoder() {
    delete root;
}

void Decoder::import(unsigned char const *data, size_t size, std::vector<unsigned char> &result) {
    if (started) {
        workWithChar(lastChar, result, false);
    } else {
        started = true;
    }
    lastChar = data[size - 1];
    size--;
    for (size_t i = 0; i < size; i++) {
        workWithChar(data[i], result, false);
    }
}

void Decoder::end(std::vector<unsigned char> &result) {
    workWithChar(lastChar, result, true);
    if (!stack.empty()) {
        throw FileCorruptedException("Tree is not completed");
    }
    if (!leafs.empty()) {
        throw FileCorruptedException("Leafs are not completed");
    }
    if (decodingNode != root) {
        throw FileCorruptedException("Last char was not decoded");
    }
}

void Decoder::workWithChar(unsigned char c, std::vector<unsigned char> &result, bool isLast) {
    if (!stack.empty()) {
        for (size_t j = 0; j < 8 && !stack.empty(); j++) {
            buildTree(c & (1u << 7u));
            c <<= 1u;
        }
    } else if (!leafs.empty()) {
        leafs.top()->setChar(c);
        leafs.pop();
    } else {
        if (bitsInLastChar == UINT8_MAX) {
            bitsInLastChar = (c & 0b11100000u) >> 5u;
            c <<= 3u;
            if (isLast) {
                if (bitsInLastChar == 0) {
                    bitsInLastChar = 8;
                }
                for (size_t j = 3; j < bitsInLastChar; j++) {
                    workWithBit(c & (1u << 7u), result);
                    c <<= 1u;
                }
                return;
            } else {
                if (bitsInLastChar == 0) {
                    bitsInLastChar = 8;
                }
                for (size_t j = 3; j < 8; j++) {
                    workWithBit(c & (1u << 7u), result);
                    c <<= 1u;
                }
                return;
            }
        }
        if (isLast) {
            for (size_t j = 0; j < bitsInLastChar; j++) {
                workWithBit(c & (1u << 7u), result);
                c <<= 1u;
            }
        } else {
            for (size_t j = 0; j < 8; j++) {
                workWithBit(c & (1u << 7u), result);
                c <<= 1u;
            }
        }
    }
}

void Decoder::workWithBit(bool b, std::vector<unsigned char> &result) {
    if (b) {
        decodingNode = decodingNode->rightChild;
    } else {
        decodingNode = decodingNode->leftChild;
    }
    if (decodingNode->isLeaf()) {
        result.push_back(decodingNode->getChar());
        decodingNode = root;
    }
}

void Decoder::buildTree(bool b) {
    if (b) {
        stack.top()->leftChild = new Node;
        stack.push(stack.top()->leftChild);
    } else {
        leafs.push(stack.top());
        stack.pop();
        while (!stack.empty() && stack.top()->rightChild != nullptr) {
            stack.pop();
        }
        if (stack.empty()) {
            if (root->isLeaf()) {
                throw FileCorruptedException("Root is leaf");
            }
            return;
        }
        stack.top()->rightChild = new Node;
        stack.push(stack.top()->rightChild);
    }
}
