#ifndef BOOKSTORE_STOCK_HPP
#define BOOKSTORE_STOCK_HPP

#include <string>
#include <memory.h>

class Book {
public:
    char ISBN[20];
    char BookName[60];
    char Author[60];
    char Keyword[60];
    double Price;
    int stock;

    Book();

    Book(const std::string &ISBN, const std::string &BookName,
         const std::string &Author, const std::string &Keyword,
         double Price, int stock);
};

#endif //BOOKSTORE_STOCK_HPP
