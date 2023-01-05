#ifndef BOOKSTORE_LOGIN_HPP
#define BOOKSTORE_LOGIN_HPP

#include <string>
#include <fstream>
#include <memory.h>
#include "error.hpp"

class User {
public:
    char UserID[30];
    char Password[30];
    char Username[30];
    int Privilege;

    User();

    User(const std::string &UserID, const std::string &Password,
         const std::string &Username, int Privilege);
};

class LoginStack {
public:
    LoginStack();

    ~LoginStack();

    std::string getUser();

    std::string getISBN();

    void login(const std::string &UserID);

    void select(const std::string &ISBN);

    void logout();

    bool checkLogin(const std::string &UserID);

    void flush(const std::string &ord, const std::string &change);

private:
    struct loginUser {
        char UserID[32];
        char ISBN[32];
        loginUser() {
            memset(UserID, 0, sizeof(UserID));
            memset(ISBN, 0, sizeof(ISBN));
        }
    };
    int usersCount;
    std::fstream file;
};

#endif //BOOKSTORE_LOGIN_HPP
