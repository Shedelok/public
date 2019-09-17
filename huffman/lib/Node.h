#ifndef HUFFMAN_NODE_H
#define HUFFMAN_NODE_H

struct Node {
    Node *leftChild = nullptr;
    Node *rightChild = nullptr;

    Node(unsigned char);

    Node();

    ~Node();

    bool isLeaf() const;

    unsigned char getChar() const;

    void setChar(unsigned char);

private:
    unsigned char c_;
};

#endif //HUFFMAN_NODE_H
