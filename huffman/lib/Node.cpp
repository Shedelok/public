#include "Node.h"

Node::Node(unsigned char c) {
    c_ = c;
}

Node::Node() : Node(0) {
}

Node::~Node() {
    delete leftChild;
    delete rightChild;
}

bool Node::isLeaf() const {
    return leftChild == nullptr;
}

unsigned char Node::getChar() const {
    return c_;
}

void Node::setChar(unsigned char c) {
    c_ = c;
}
