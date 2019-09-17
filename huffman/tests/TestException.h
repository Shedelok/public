#ifndef HUFFMAN_TESTEXCEPTION_H
#define HUFFMAN_TESTEXCEPTION_H

#include <stdexcept>

struct TestException : std::runtime_error {
    explicit TestException(std::string const &message);
};

#endif //HUFFMAN_TESTEXCEPTION_H
