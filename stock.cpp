#include "stock.hpp"

Book::Book() {
    memset(ISBN, 0, sizeof(ISBN));
    memset(BookName, 0, sizeof(BookName));
    memset(Author, 0, sizeof(Author));
    memset(Keyword, 0, sizeof(Keyword));
    Price = 0;
    stock = 0;
}

Book::Book(const std::string &ISBN_, const std::string &BookName_,
           const std::string &Author_, const std::string &Keyword_,
           double Price_, int stock_) {
    memset(ISBN, 0, sizeof(ISBN));
    memset(BookName, 0, sizeof(BookName));
    memset(Author, 0, sizeof(Author));
    memset(Keyword, 0, sizeof(Keyword));
    for (int i = 0; i < ISBN_.length(); ++i) { ISBN[i] = ISBN_[i]; }
    for (int i = 0; i < BookName_.length(); ++i) { BookName[i] = BookName_[i]; }
    for (int i = 0; i < Author_.length(); ++i) { Author[i] = Author_[i]; }
    for (int i = 0; i < Keyword_.length(); ++i) { Keyword[i] = Keyword_[i]; }
    Price = Price_;
    stock = stock_;
}

