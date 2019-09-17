#ifndef HUFFMAN_FILENOTFOUNDEXCEPTION_H
#define HUFFMAN_FILENOTFOUNDEXCEPTION_H

#include <stdexcept>

struct FileNotFoundException : std::runtime_error {
    explicit FileNotFoundException(std::string const &message);
};


#endif //HUFFMAN_FILENOTFOUNDEXCEPTION_H
