#ifndef BOOKSTORE_LOG_HPP
#define BOOKSTORE_LOG_HPP

#include <fstream>
#include <iostream>
#include "memory.h"
#include "error.hpp"

class Log {
public:
    Log();

    int get_count();

    int get_log_count();

    void add_finance(double val);

    void show_finance(int count = -1);

    void add_log(const std::string &str);

    void show_log();

private:
    std::fstream file;
};

#endif //BOOKSTORE_LOG_HPP
