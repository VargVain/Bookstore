#include <iostream>
#include "database.hpp"
#include "error.hpp"
#include "tokenScanner.hpp"
#include "operation.hpp"
#include "login.hpp"
#include "stock.hpp"
#include "log.hpp"
#include "iomanip"

int main() {
    Database<User> usersData("users");
    Database<Book> booksData("books");
    Log log;
    if (!usersData.find("root").Privilege) {
        User root("root", "sjtu", "root", 7);
        usersData.insert("root", root);
    }
    LoginStack login;
    Operation *op = nullptr;
    std::cout << std::fixed << std::setprecision(2);
    //std::ios::sync_with_stdio(false);
    //std::cin.tie(nullptr);
    //std::cout.tie(nullptr);
    while (true) {
        try {
            std::string input;
            getline(std::cin, input);
            TokenScanner scanner(input);
            std::string key = scanner.nextToken();
            std::string second = scanner.nextToken();
            if (key.empty()) continue;
            //else if (key == "test") op = new op_test(input);
            else if (key == "quit" || key == "exit") op = new op_quit(input);
            else if (key == "su") op = new op_su(input);
            else if (key == "logout") op = new op_logout(input);
            else if (key == "register") op = new op_register(input);
            else if (key == "passwd") op = new op_passwd(input);
            else if (key == "useradd") op = new op_useradd(input);
            else if (key == "delete") op = new op_delete(input);
            else if (key == "show" && second != "finance") op = new op_show(input);
            else if (key == "buy") op = new op_buy(input);
            else if (key == "select") op = new op_select(input);
            else if (key == "modify") op = new op_modify(input);
            else if (key == "import") op = new op_import(input);
            else if (key == "show" && second == "finance") op = new op_show_finance(input);
            //else if (key == "log") op = new op_log(input);
            else {
                error();
                continue;
            }
            op->check(login, usersData, booksData, log);
            op->execute(login, usersData, booksData, log);
            delete op;
            op = nullptr;
        } catch (ErrorException &ex) {
            std::cout << "Invalid" << std::endl;
            delete op;
            op = nullptr;
        }
    }
}