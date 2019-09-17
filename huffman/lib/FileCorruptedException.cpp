#include "FileCorruptedException.h"

FileCorruptedException::FileCorruptedException(std::string const &message) : std::runtime_error(message) {
}
