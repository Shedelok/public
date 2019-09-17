cmake_minimum_required(VERSION 3.14)
project(huffman)

set(CMAKE_CXX_STANDARD 14)

add_executable(huffman huffman.cpp
        tests/Tester.h tests/Tester.cpp tests/TestException.h tests/TestException.cpp
        utility/Compressor.h utility/Compressor.cpp utility/Unpacker.h utility/Unpacker.cpp
        lib/Node.h lib/Node.cpp lib/FrequencyCounter.cpp lib/FrequencyCounter.h lib/Tree.cpp lib/Tree.h lib/BitSequence.cpp lib/BitSequence.h lib/Encoder.cpp lib/Encoder.h lib/Decoder.cpp lib/Decoder.h lib/FileCorruptedException.cpp lib/FileCorruptedException.h lib/FileNotFoundException.cpp lib/FileNotFoundException.h)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto")
endif()
