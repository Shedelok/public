#ifndef HUFFMAN_TESTER_H
#define HUFFMAN_TESTER_H

#include <functional>
#include <memory.h>
#include "../utility/Compressor.h"
#include "../utility/Unpacker.h"
#include "TestException.h"

struct Tester {
    static void run(bool);

private:
    static char const *const INPUT_FILE_NAME;
    static char const *const COMPRESSED_FILE_NAME;
    static char const *const OUTPUT_FILE_NAME;
    static unsigned char const RANDOM_PERMUTATION[];

    static size_t testNumber;
    static size_t totalGroups;
    static size_t passedGroups;
    static bool mode;

    static void pretests();

    static void emptyFile();

    static void smallTests();

    static void middleTests();

    static void hugeTests();

    static void rootIsLeaf();

    static void treeIsNotCompleted();

    static void leafsAreNotCompleted();

    static void lastCharWasNotDecoded();

    static void applyTest(std::function<void(FILE *)> const &);

    static void applyExceptionTest(std::function<void(FILE *)> const &);

    static void applyGroup(void (*)(), char const *);

    static std::pair<float, char const *> getFileSize(size_t);
};


#endif //HUFFMAN_TESTER_H
