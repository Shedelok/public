#include "TestException.h"

TestException::TestException(std::string const &message) : std::runtime_error(message) {
}
