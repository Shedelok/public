#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include <queue>
#include "FrequencyCounter.h"
#include "BitSequence.h"

struct Tree {
    explicit Tree(FrequencyCounter const &);

    ~Tree();

    BitSequence toBitSequence() const;

    Node *getRoot() const;

    FrequencyCounter const *getFrequencyCounter() const;

private:
    Node *root;
    FrequencyCounter const *frequencyCounter;

    static void toBitSequence(Node *, BitSequence &);

    static void codeLeafs(Node *, BitSequence &);
};

#endif //HUFFMAN_TREE_H
