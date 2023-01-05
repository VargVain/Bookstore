#include <string>
#include "error.hpp"

ErrorException::ErrorException() = default;

void error() {
    throw ErrorException();
}
