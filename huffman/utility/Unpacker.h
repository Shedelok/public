#ifndef HUFFMAN_UNPACKER_H
#define HUFFMAN_UNPACKER_H

#include <cstdio>
#include <vector>
#include <stdexcept>
#include "../lib/Decoder.h"
#include "../lib/FileCorruptedException.h"
#include "../lib/FileNotFoundException.h"

struct Unpacker {
    static void unpack(const char *, const char *);
};

#endif //HUFFMAN_UNPACKER_H
