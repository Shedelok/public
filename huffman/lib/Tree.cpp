#include "Tree.h"

Tree::Tree(FrequencyCounter const &fc) {
    frequencyCounter = &fc;
    std::priority_queue<std::pair<size_t, Node *>,
            std::vector<std::pair<size_t, Node *>>,
            std::greater<>> queue;
    for (size_t i = 0; i < 256; i++) {
        if (fc[i] != 0) {
            queue.push(std::make_pair(fc[i], new Node(i)));
        }
    }
    while (queue.size() > 1) {
        auto p1 = queue.top();
        queue.pop();
        auto p2 = queue.top();
        queue.pop();
        Node *node = new Node;
        node->leftChild = p1.second;
        node->rightChild = p2.second;
        queue.push(std::make_pair(p1.first + p2.first, node));
    }
    root = queue.empty() ? new Node : queue.top().second;
    if (root->isLeaf()) {
        root->leftChild = new Node(root->getChar());
        root->rightChild = new Node(root->getChar());
    }
}

Tree::~Tree() {
    delete root;
}

BitSequence Tree::toBitSequence() const {
    BitSequence result;
    for (Node *n = root; !n->isLeaf(); n = n->rightChild) {
        result.add(true);
        toBitSequence(n->leftChild, result);
    }
    result.add(false);
    codeLeafs(root, result);
    return result;
}

Node *Tree::getRoot() const {
    return root;
}

FrequencyCounter const *Tree::getFrequencyCounter() const {
    return frequencyCounter;
}

void Tree::toBitSequence(Node *node, BitSequence &result) {
    if (!node->isLeaf()) {
        result.add(true);
        toBitSequence(node->leftChild, result);
        toBitSequence(node->rightChild, result);
    } else {
        result.add(false);
    }
}

void Tree::codeLeafs(Node *node, BitSequence &result) {
    if (node->isLeaf()) {
        result.add(node->getChar());
        return;
    }
    codeLeafs(node->rightChild, result);
    codeLeafs(node->leftChild, result);
}
