#include "Compressor.h"

void Compressor::compress(const char *inputFileName, const char *outputFileName) {
    FrequencyCounter frequencyCounter;
    FILE *input = fopen(inputFileName, "rb");
    if (!input) {
        throw FileNotFoundException("Input file to compress does not exist");
    }
    unsigned char buffer[256];
    while (!feof(input)) {
        size_t size = fread(buffer, sizeof(unsigned char), 256, input);
        if (ferror(input)) {
            throw FileCorruptedException("Ferror");
        }
        if (size != 0) {
            frequencyCounter.import(buffer, size);
        }
    }
    fclose(input);
    Tree tree(frequencyCounter);
    auto encoder = Encoder(tree);
    std::vector<unsigned char> toPrint;
    input = fopen(inputFileName, "rb");
    FILE *output = fopen(outputFileName, "wb");
    while (!feof(input)) {
        size_t size = fread(buffer, sizeof(unsigned char), 256, input);
        if (ferror(input)) {
            throw FileCorruptedException("Ferror");
        }
        if (size != 0) {
            encoder.import(buffer, size, toPrint);
            fwrite(toPrint.data(), sizeof(unsigned char), toPrint.size(), output);
            toPrint.clear();
        }
    }
    encoder.end(toPrint);
    fwrite(toPrint.data(), sizeof(unsigned char), toPrint.size(), output);
    fclose(input);
    fclose(output);
}
