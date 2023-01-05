#ifndef BOOKSTORE_ERROR_HPP
#define BOOKSTORE_ERROR_HPP

#include <string>
#include <exception>

class ErrorException : public std::exception {
public:
    explicit ErrorException();
};

void error();

#endif //BOOKSTORE_ERROR_HPP
