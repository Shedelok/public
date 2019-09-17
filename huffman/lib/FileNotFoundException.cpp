#include "FileNotFoundException.h"

FileNotFoundException::FileNotFoundException(std::string const &message) : std::runtime_error(message) {
}
