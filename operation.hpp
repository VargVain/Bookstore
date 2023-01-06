#ifndef BOOKSTORE_OPERATION_HPP
#define BOOKSTORE_OPERATION_HPP

#include <string>
#include <iostream>
#include "database.hpp"
#include "login.hpp"
#include "stock.hpp"
#include "tokenScanner.hpp"
#include "error.hpp"
#include "log.hpp"

class Operation {
public:
    Operation();

    virtual ~Operation() = default;

    virtual void check(LoginStack &, Database<User> &, Database<Book> &, Log &) = 0;

    virtual void execute(LoginStack &, Database<User> &, Database<Book> &, Log &) = 0;
};

class op_test : public Operation{
public:
    explicit op_test(const std::string &str);

    ~op_test() override = default;

    void check(LoginStack &, Database<User> &, Database<Book> &, Log &) override;

    void execute(LoginStack &, Database<User> &, Database<Book> &, Log &) override;

private:
    std::string input;
};

class op_quit : public Operation{
public:
    explicit op_quit(const std::string &str);

    ~op_quit() override = default;

    void check(LoginStack &, Database<User> &, Database<Book> &, Log &) override;

    void execute(LoginStack &, Database<User> &, Database<Book> &, Log &) override;

private:
    std::string input;
};

class op_su : public Operation{
public:
    explicit op_su(const std::string &str);

    ~op_su() override = default;

    void check(LoginStack &, Database<User> &, Database<Book> &, Log &) override;

    void execute(LoginStack &, Database<User> &, Database<Book> &, Log &) override;

private:
    std::string input;
    int count;
    std::string user;
    std::string password;
};

class op_logout : public Operation{
public:
    explicit op_logout(const std::string &str);

    ~op_logout() override = default;

    void check(LoginStack &, Database<User> &, Database<Book> &, Log &) override;

    void execute(LoginStack &, Database<User> &, Database<Book> &, Log &) override;

private:
    std::string input;
};

class op_register : public Operation{
public:
    explicit op_register(const std::string &str);

    ~op_register() override = default;

    void check(LoginStack &, Database<User> &, Database<Book> &, Log &) override;

    void execute(LoginStack &, Database<User> &, Database<Book> &, Log &) override;

private:
    std::string input;
    std::string userID;
    std::string password;
    std::string username;
};

class op_passwd : public Operation{
public:
    explicit op_passwd(const std::string &str);

    ~op_passwd() override = default;

    void check(LoginStack &, Database<User> &, Database<Book> &, Log &) override;

    void execute(LoginStack &, Database<User> &, Database<Book> &, Log &) override;

private:
    std::string input;
    int count;
    std::string user;
    std::string currentPassword;
    std::string newPassword;
};

class op_useradd : public Operation{
public:
    explicit op_useradd(const std::string &str);

    ~op_useradd() override = default;

    void check(LoginStack &, Database<User> &, Database<Book> &, Log &) override;

    void execute(LoginStack &, Database<User> &, Database<Book> &, Log &) override;

private:
    std::string input;
    std::string userID;
    std::string password;
    std::string username;
    int privilege;
};

class op_delete : public Operation{
public:
    explicit op_delete(const std::string &str);

    ~op_delete() override = default;

    void check(LoginStack &, Database<User> &, Database<Book> &, Log &) override;

    void execute(LoginStack &, Database<User> &, Database<Book> &, Log &) override;

private:
    std::string input;
    std::string userID;
};

class op_show : public Operation{
public:
    explicit op_show(const std::string &str);

    ~op_show() override = default;

    void check(LoginStack &, Database<User> &, Database<Book> &, Log &) override;

    void execute(LoginStack &, Database<User> &, Database<Book> &, Log &) override;

private:
    std::string input;
    int count;
    std::string para;
    std::string val;
};

class op_buy : public Operation{
public:
    explicit op_buy(const std::string &str);

    ~op_buy() override = default;

    void check(LoginStack &, Database<User> &, Database<Book> &, Log &) override;

    void execute(LoginStack &, Database<User> &, Database<Book> &, Log &) override;

private:
    std::string input;
    std::string ISBN;
    int Quantity;
};

class op_select : public Operation{
public:
    explicit op_select(const std::string &str);

    ~op_select() override = default;

    void check(LoginStack &, Database<User> &, Database<Book> &, Log &) override;

    void execute(LoginStack &, Database<User> &, Database<Book> &, Log &) override;

private:
    std::string input;
    std::string ISBN;
};

class op_modify : public Operation{
public:
    explicit op_modify(const std::string &str);

    ~op_modify() override = default;

    void check(LoginStack &, Database<User> &, Database<Book> &, Log &) override;

    void execute(LoginStack &, Database<User> &, Database<Book> &, Log &) override;

private:
    std::string input;
    std::string ISBN;
};

class op_import : public Operation{
public:
    explicit op_import(const std::string &str);

    ~op_import() override = default;

    void check(LoginStack &, Database<User> &, Database<Book> &, Log &) override;

    void execute(LoginStack &, Database<User> &, Database<Book> &, Log &) override;

private:
    std::string input;
    int Quantity;
    float TotalCost;
};

class op_show_finance : public Operation{
public:
    explicit op_show_finance(const std::string &str);

    ~op_show_finance() override = default;

    void check(LoginStack &, Database<User> &, Database<Book> &, Log &) override;

    void execute(LoginStack &, Database<User> &, Database<Book> &, Log &) override;

private:
    std::string input;
    int count;
    int fcount;
};

class op_log : public Operation{
public:
    explicit op_log(const std::string &str);

    ~op_log() override = default;

    void check(LoginStack &, Database<User> &, Database<Book> &, Log &) override;

    void execute(LoginStack &, Database<User> &, Database<Book> &, Log &) override;

private:
    std::string input;
};

#endif //BOOKSTORE_OPERATION_HPP
