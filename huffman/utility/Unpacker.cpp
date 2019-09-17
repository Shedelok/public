#include "Unpacker.h"

void Unpacker::unpack(const char *inputFileName, const char *outputFileName) {
    std::vector<unsigned char> toPrint;
    unsigned char buffer[256];
    Decoder decoder;
    FILE *input = fopen(inputFileName, "rb");
    if (!input) {
        throw FileNotFoundException("Input file to unpack does not exist");
    }
    FILE *output = fopen(outputFileName, "wb");
    while (!feof(input)) {
        size_t size = fread(&buffer, sizeof(unsigned char), 256, input);
        if (ferror(input)) {
            throw FileCorruptedException("Ferror");
        }
        if (size != 0) {
            decoder.import(buffer, size, toPrint);
            if (!toPrint.empty()) {
                fwrite(toPrint.data(), sizeof(unsigned char), toPrint.size(), output);
                toPrint.clear();
            }
        }
    }
    decoder.end(toPrint);
    if (!toPrint.empty()) {
        fwrite(toPrint.data(), sizeof(unsigned char), toPrint.size(), output);
    }
    fclose(input);
    fclose(output);
}

