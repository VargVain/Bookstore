#ifndef BOOKSTORE_TOKENSCANNER_HPP
#define BOOKSTORE_TOKENSCANNER_HPP

#include <string>
#include "error.hpp"

class TokenScanner {
public:
    TokenScanner();

    TokenScanner(const std::string &str);

    ~TokenScanner();

    std::string nextToken();

    int getTokenCount();

private:
    std::string input;
    int position;
};

std::string checkPara(const std::string &str);

std::string checkVal(const std::string &str, const std::string &para);

bool consist(const std::string &str, const std::string &sum);

void check(const std::string &input, const std::string &para);

#endif //BOOKSTORE_TOKENSCANNER_HPP
