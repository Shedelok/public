#ifndef HUFFMAN_COMPRESSOR_H
#define HUFFMAN_COMPRESSOR_H

#include <cstdio>
#include "../lib/FrequencyCounter.h"
#include "../lib/Tree.h"
#include "../lib/Encoder.h"
#include "../lib/FileNotFoundException.h"
#include "../lib/FileCorruptedException.h"

struct Compressor {
    static void compress(const char *, const char *);
};


#endif //HUFFMAN_COMPRESSOR_H
