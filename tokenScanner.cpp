#include "tokenScanner.hpp"

TokenScanner::TokenScanner() : position(0) {}

TokenScanner::TokenScanner(const std::string &str) : input(str), position(0) {}

TokenScanner::~TokenScanner() = default;

std::string TokenScanner::nextToken() {
    if (input.empty() || position == input.length() - 1) return "";
    int st;
    while (position < input.length() - 1 && input[position] == ' ') { ++position; }
    st = position;
    while (position < input.length() - 1 && input[position] != ' ') { ++position; }
    if (input[position] == ' ') return input.substr(st, position - st);
    else return input.substr(st, position - st + 1);
}

int TokenScanner::getTokenCount() {
    bool space = true;
    int out = 0;
    for (int i = 0; i < input.length(); ++i) {
        if (space && input[i] != ' ') {
            space = false;
            ++out;
        }
        else if (!space && input[i] == ' ') {
            space = true;
        }
    }
    return out;
}

std::string checkPara(const std::string &str) {
    if (str.length() <= 6) error();
    if (str.substr(0, 6) == "-ISBN=") return "ISBN";
    if (str.substr(0, 6) == "-name=") return "name";
    if (str.length() <= 7) error();
    if (str.substr(0, 7) == "-price=") return "price";
    if (str.length() <= 8) error();
    if (str.substr(0, 8) == "-author=") return "author";
    if (str.length() <= 9) error();
    if (str.substr(0, 9) == "-keyword=") return "keyword";
    error();
    return "";
}

std::string checkVal(const std::string &str, const std::string &para) {
    if (para == "ISBN") {
        return str.substr(6);
    }
    if (para == "price") {
        return str.substr(7);
    }
    if (para == "name") {
        if (str[6] != '"' || str[str.length() - 1] != '"') error();
        if (str.length() < 9) error();
        return str.substr(7, str.length() - 8);
    }
    if (para == "author") {
        if (str[8] != '"' || str[str.length() - 1] != '"') error();
        if (str.length() < 11) error();
        return str.substr(9, str.length() - 10);
    }
    if (para == "keyword") {
        if (str[9] != '"' || str[str.length() - 1] != '"') error();
        if (str.length() < 12) error();
        return str.substr(10, str.length() - 11);
    }
    error();
    return "";
}

bool consist(const std::string &str, const std::string &sum) {
    std::string check;
    for (int i = 0; i < sum.length(); ++i) {
        if (sum[i] == '|') {
            if (check == str) return true;
            check.clear();
        }
        else {
            check.push_back(sum[i]);
        }
    }
    if (check == str) return true;
    return false;
}

void check(const std::string &input, const std::string &para) {
    if (para == "uop") {
        if (input.length() > 30) error();
        for (int i = 0; i < input.length(); ++i) {
            if (!isalnum(input[i]) && input[i] != '_') error();
        }
        return;
    }
    if (para == "username") {
        if (input.length() > 30) error();
        return;
    }
    if (para == "pvl") {
        if (input.length() > 1) error();
        if (input[0] != '0' && input[0] != '1' && input[0] != '3' && input[0] != '7') error();
        return;
    }
    if (para == "ISBN") {
        if (input.length() > 20) error();
        return;
    }
    if (para == "name" || para == "author") {
        if (input.length() > 60) error();
        for (int i = 0; i < input.length(); ++i) {
            if (input[i] == '\"') error();
        }
        return;
    }
    if (para == "keyword") {
        if (input.length() > 60) error();
        for (int i = 0; i < input.length(); ++i) {
            if (input[i] == '\"') error();
        }
        std::string check;
        for (int i = 0; i < input.length(); ++i) {
            if (input[i] == '|') {
                if (consist(check, input.substr(i + 1))) error();
                check.clear();
            }
            else {
                check.push_back(input[i]);
            }
        }
        return;
    }
    if (para == "quantity") {
        if (input.length() > 10) error();
        for (int i = 0; i < input.length(); ++i) {
            if (!isdigit(input[i])) error();
        }
        return;
    }
    if (para == "price") {
        if (input.length() > 10) error();
        for (int i = 0; i < input.length(); ++i) {
            if (!isdigit(input[i]) && input[i] != '.') error();
        }
        return;
    }
}
