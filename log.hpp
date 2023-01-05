#ifndef BOOKSTORE_LOG_HPP
#define BOOKSTORE_LOG_HPP

#include <fstream>
#include <iostream>
#include "error.hpp"

class Log {
public:
    Log();

    int get_count();

    void add_finance(double val);

    void show_finance(int count = -1);

private:
    std::fstream file;
};

#endif //BOOKSTORE_LOG_HPP
