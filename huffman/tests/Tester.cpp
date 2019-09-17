#include "Tester.h"
#include "../lib/FileCorruptedException.h"

char const *const Tester::INPUT_FILE_NAME = "test.in";
char const *const Tester::COMPRESSED_FILE_NAME = "test.compressed";
char const *const Tester::OUTPUT_FILE_NAME = "test.out";

#define ANSI_RESET "\x1b[0m"
#define RED "\x1b[31m"
#define BRIGHT_RED "\x1b[91m"
#define GREEN "\x1b[32m"
#define BRIGHT_GREEN "\x1b[92m"
#define GREEN_BOLD "\x1b[32;1m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"

#define printArray(arr, file) fwrite(arr, sizeof(unsigned char), sizeof(arr) / sizeof(unsigned char), file)

size_t Tester::testNumber;
size_t Tester::totalGroups;
size_t Tester::passedGroups;
bool Tester::mode;
unsigned char const Tester::RANDOM_PERMUTATION[] =
        {99, 63, 119, 148, 112, 136, 117, 226, 65, 16, 102, 21, 75, 152, 66, 243,
         105, 216, 85, 210, 88, 193, 73, 42, 87, 12, 74, 218, 114, 171, 100, 187,
         81, 192, 68, 140, 108, 245, 78, 229, 109, 93, 101, 238, 115, 34, 116, 144,
         71, 54, 98, 161, 113, 202, 79, 32, 77, 2, 72, 26, 86, 164, 70, 56,
         111, 223, 103, 19, 83, 199, 90, 196, 120, 236, 118, 233, 67, 0, 84, 250,
         110, 92, 104, 169, 121, 253, 97, 190, 107, 206, 82, 184, 106, 29, 89, 1,
         80, 13, 76, 41, 122, 189, 69, 200, 173, 177, 6, 150, 123, 163, 166, 20,
         225, 141, 135, 232, 8, 255, 240, 137, 191, 129, 47, 197, 46, 209, 201, 217,
         38, 94, 130, 154, 220, 156, 241, 254, 183, 176, 198, 244, 33, 155, 31, 212,
         133, 28, 151, 213, 17, 231, 208, 235, 44, 45, 228, 124, 153, 188, 195, 128,
         186, 49, 178, 247, 246, 27, 157, 146, 159, 165, 62, 248, 221, 125, 160, 239,
         205, 167, 179, 175, 170, 194, 35, 126, 55, 95, 252, 58, 51, 132, 37, 214,
         219, 207, 7, 5, 127, 30, 40, 242, 91, 3, 174, 57, 10, 23, 48, 39,
         131, 53, 134, 60, 36, 185, 224, 249, 234, 172, 138, 181, 147, 11, 237, 18,
         4, 180, 96, 168, 158, 162, 204, 149, 15, 22, 215, 182, 143, 211, 222, 64,
         230, 24, 52, 59, 14, 251, 50, 43, 145, 227, 25, 203, 139, 61, 9, 142};

void Tester::run(bool showAddInfo) {
    mode = showAddInfo;
    testNumber = totalGroups = passedGroups = 0;
    applyGroup(Tester::pretests, "pretests");
    applyGroup(Tester::emptyFile, "empty file");
    applyGroup(Tester::smallTests, "small tests");
    applyGroup(Tester::middleTests, "middle-size tests");
    applyGroup(Tester::hugeTests, "huge tests");
    applyGroup(Tester::rootIsLeaf, "root is leaf");
    applyGroup(Tester::treeIsNotCompleted, "tree is not completed");
    applyGroup(Tester::leafsAreNotCompleted, "leafs are not completed");
    applyGroup(Tester::lastCharWasNotDecoded, "last char was not decoded");
    printf("====================================\nTotal groups passed: %zu. Failed: %zu\nTotal tests passed: %zu\n",
           passedGroups, totalGroups - passedGroups, testNumber);
}

void Tester::pretests() {
    applyTest([](FILE *file) {
        unsigned char buf[] = {'a', 'b', 'a'};
        printArray(buf, file);
    });
    applyTest([](FILE *file) {
        unsigned char buf[] = {'a', 'b', 'a', 'c', 'a', 'b', 'a'};
        printArray(buf, file);
    });
    for (size_t i = 1; i <= 8; i++) {
        applyTest([&i](FILE *file) {
            unsigned char buf[i];
            memset(buf, RANDOM_PERMUTATION[i], sizeof(buf));
            printArray(buf, file);
        });
    }
}

void Tester::emptyFile() {
    applyTest([](FILE *file) {
    });
}

void Tester::smallTests() {
    for (size_t i = 9; i <= 256; i++) {
        applyTest([&i](FILE *file) {
            unsigned char buf[i];
            memset(buf, RANDOM_PERMUTATION[i - 1], sizeof(buf));
            printArray(buf, file);
        });
    }
    for (size_t i = 1; i <= 256; i++) {
        applyTest([&i](FILE *file) {
            fwrite(&RANDOM_PERMUTATION, sizeof(unsigned char), i, file);
        });
    }
}

void Tester::middleTests() {
    for (size_t i = 2; i <= 257; i++) {
        applyTest([&i](FILE *file) {
            unsigned char buf[256 * i];
            for (size_t j = 0; j < 256; j++) {
                for (size_t k = 0; k < i; k++) {
                    buf[j * i + k] = RANDOM_PERMUTATION[j];
                }
            }
            printArray(buf, file);
        });
        applyTest([&i](FILE *file) {
            unsigned char buf[256 * i];
            for (size_t k = 0; k < i; k++) {
                for (size_t j = 0; j < 256; j++) {
                    buf[j * i + k] = RANDOM_PERMUTATION[j];
                }
            }
            printArray(buf, file);
        });
    }
    applyTest([](FILE *file) {
        unsigned char buf[123217];
        memset(buf, RANDOM_PERMUTATION[0], sizeof(buf));
        printArray(buf, file);
    });
    applyTest([](FILE *file) {
        unsigned char buf[134180];
        memset(buf, RANDOM_PERMUTATION[1], sizeof(buf));
        buf[134180 - 1] = RANDOM_PERMUTATION[2];
        printArray(buf, file);
    });
    applyTest([](FILE *file) {
        unsigned char buf[413856];
        for (size_t i = 0; i < 413856; i++) {
            buf[i] = RANDOM_PERMUTATION[10] + (RANDOM_PERMUTATION[i % 256] % 2);
        }
        printArray(buf, file);
    });
}

void Tester::hugeTests() {
    for (size_t mb = 1; mb <= 100; mb *= 10) {
        applyTest([&mb](FILE *file) {
            for (size_t i = 0; i < 1000000 * mb; i++) {
                fwrite(&RANDOM_PERMUTATION[228], sizeof(unsigned char), 1, file);
            }
        });
    }
    for (size_t mb = 1; mb <= 100; mb *= 10) {
        applyTest([&mb](FILE *file) {
            for (size_t i = 0; i < 1000000 * mb; i++) {
                fwrite(&RANDOM_PERMUTATION[i % 256], sizeof(unsigned char), 1, file);
            }
        });
    }
}

void Tester::rootIsLeaf() {
    applyExceptionTest([](FILE *file) {
        unsigned char c = 0;
        fwrite(&c, sizeof(unsigned char), 1, file);
    });
}

void Tester::treeIsNotCompleted() {
    applyExceptionTest([](FILE *file) {
        unsigned char c = 0b11111111;
        fwrite(&c, sizeof(unsigned char), 1, file);
    });
    applyExceptionTest([](FILE *file) {
        unsigned char c = 0b11101100;
        fwrite(&c, sizeof(unsigned char), 1, file);
    });
    applyExceptionTest([](FILE *file) {
        unsigned char c = 0b11110000;
        fwrite(&c, sizeof(unsigned char), 1, file);
    });
}

void Tester::leafsAreNotCompleted() {
    applyExceptionTest([](FILE *file) {
        unsigned char buf[] = {0b10000000, 0};
        printArray(buf, file);
    });
}

void Tester::lastCharWasNotDecoded() {
    applyExceptionTest([](FILE *file) {
        unsigned char buf[] = {0b11001000, 'a', 'b', 'c', 'd', 0};
        printArray(buf, file);
    });
}

void Tester::applyTest(std::function<void(FILE *)> const &printTest) {
    ++testNumber;
    FILE *input = fopen(INPUT_FILE_NAME, "wb");
    printTest(input);
    fclose(input);
    auto startTime = clock();
    Compressor::compress(INPUT_FILE_NAME, COMPRESSED_FILE_NAME);
    auto compressedTime = clock();
    Unpacker::unpack(COMPRESSED_FILE_NAME, OUTPUT_FILE_NAME);
    auto unpackedTime = clock();
    input = fopen(INPUT_FILE_NAME, "rb");
    FILE *output = fopen(OUTPUT_FILE_NAME, "rb");
    while (true) {
        unsigned char c1, c2;
        fread(&c1, 1, 1, input);
        fread(&c2, 1, 1, output);
        if (feof(input) || feof(output)) {
            if (!feof(input) || !feof(output)) {
                throw TestException(std::to_string(testNumber));
            }
            fclose(output);
            if (mode) {
                FILE *compressed = fopen(COMPRESSED_FILE_NAME, "rb");
                fseek(compressed, 0, SEEK_END);
                std::pair<float, char const *> inputSize = getFileSize(ftell(input));
                std::pair<float, char const *> compressedSize = getFileSize(ftell(compressed));
                float percent = 100. * ftell(compressed) / ftell(input);
                printf(GREEN_BOLD "OK" ANSI_RESET " | "
                       BLUE "%.2f %s" ANSI_RESET " | "
                       BLUE "%.2f %s" ANSI_RESET " | "
                       "%s %.1f%%" ANSI_RESET " | "
                       BLUE "%.2fs" ANSI_RESET " | "
                       BLUE "%.2fs" ANSI_RESET " | "
                       BLUE "%.2fs" ANSI_RESET "\n",
                       inputSize.first,
                       inputSize.second,
                       compressedSize.first,
                       compressedSize.second,
                       percent > 400 ? RED :
                       percent > 100 ? BRIGHT_RED :
                       percent > 80 ? YELLOW :
                       percent > 40 ? GREEN
                                    : BRIGHT_GREEN,
                       percent,
                       1. * (compressedTime - startTime) / CLOCKS_PER_SEC,
                       1. * (unpackedTime - compressedTime) / CLOCKS_PER_SEC,
                       1. * (unpackedTime - startTime) / CLOCKS_PER_SEC);
                fclose(compressed);
            }
            fclose(input);
            return;
        }
        if (c1 != c2) {
            fclose(input);
            fclose(output);
            throw TestException(std::to_string(testNumber));
        }
    }
}

void Tester::applyExceptionTest(std::function<void(FILE *)> const &printTest) {
    ++testNumber;
    FILE *compressed = fopen(COMPRESSED_FILE_NAME, "wb");
    printTest(compressed);
    fclose(compressed);
    try {
        Unpacker::unpack(COMPRESSED_FILE_NAME, OUTPUT_FILE_NAME);
    } catch (FileCorruptedException const &e) {
        if (mode) {
            printf(GREEN_BOLD "OK" ANSI_RESET " | Exception catched: %s\n", e.what());
        }
        return;
    }
    throw TestException(std::to_string(testNumber));
}

void Tester::applyGroup(void (*testGroup)(), char const *groupName) {
    totalGroups++;
    printf("========= Testing %s ==========\n", groupName);
    size_t prevTestNumber = testNumber;
    try {
        testGroup();
    } catch (TestException const &e) {
        printf(RED "[group failed]: %s " RED "on test %s" ANSI_RESET "\n", groupName,
               std::to_string(std::stoi(e.what()) - prevTestNumber).c_str());
        return;
    }
    passedGroups++;
    printf(GREEN "[group passed]:" ANSI_RESET " %s\n", groupName);
}

std::pair<float, char const *> Tester::getFileSize(size_t bytes) {
    if (bytes < 500) {
        return std::make_pair(bytes, "bytes");
    }
    float tmp = bytes / 1000.;
    if (tmp < 500) {
        return std::make_pair(tmp, "kB");
    }
    tmp /= 1000.;
    if (tmp < 500) {
        return std::make_pair(tmp, "MB");
    }
    return std::make_pair(tmp, "GB");
}
