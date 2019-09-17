#ifndef HUFFMAN_FILECORRUPTEDEXCEPTION_H
#define HUFFMAN_FILECORRUPTEDEXCEPTION_H

#include <stdexcept>

struct FileCorruptedException: std::runtime_error {
    explicit FileCorruptedException(std::string const &message);
};

#endif //HUFFMAN_FILECORRUPTEDEXCEPTION_H
