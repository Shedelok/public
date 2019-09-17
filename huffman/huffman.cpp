#include <cstdio>
#include <stdexcept>
#include "utility/Compressor.h"
#include "utility/Unpacker.h"
#include "tests/Tester.h"
#include "lib/FileCorruptedException.h"
#include "lib/FileNotFoundException.h"

char const *TEST = "test";
char const *FILE_TO_CODE = "compress";
char const *CODE_TO_FILE = "unpack";
char const *TEST_WITH_MORE_INFORMATION = "-show";

int main(int argc, char *argv[]) {
    if (argc >= 2 && strcmp(argv[1], TEST) == 0) {
        Tester::run(argc >= 3 && strcmp(argv[2], TEST_WITH_MORE_INFORMATION) == 0);
        return 0;
    }
    if (argc != 4 || (strcmp(argv[1], FILE_TO_CODE) != 0 && strcmp(argv[1], CODE_TO_FILE) != 0)) {
        fprintf(stderr,
                "Usage: %s [input file name] [output file name]\n       %s [input file name] [output file name]\n"
                "       %s\n       %s -show\n",
                FILE_TO_CODE, CODE_TO_FILE, TEST, TEST);
        return 0;
    }
    try {
        if (strcmp(argv[1], FILE_TO_CODE) == 0) {
            Compressor::compress(argv[2], argv[3]);
        } else {
            Unpacker::unpack(argv[2], argv[3]);
        }
    } catch (FileCorruptedException const &e) {
        fprintf(stderr, "File is corrupted. %s\n", e.what());
    } catch (FileNotFoundException const &e) {
        fprintf(stderr, "File is not found. %s\n", e.what());
    }
}
