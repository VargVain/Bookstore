#include "operation.hpp"

Operation::Operation() = default;

op_test::op_test(const std::string &str) { input = str; }

void op_test::check(LoginStack &, Database<User> &, Database<Book> &, Log &) {
    TokenScanner scanner(input);
    if (scanner.getTokenCount() != 1) error();
}

void op_test::execute(LoginStack &, Database<User>& dtb, Database<Book> &, Log &) {
}

op_quit::op_quit(const std::string &str) { input = str; }

void op_quit::check(LoginStack &, Database<User> &, Database<Book> &, Log &) {
    TokenScanner scanner(input);
    if (scanner.getTokenCount() != 1) error();
}

void op_quit::execute(LoginStack &, Database<User> &, Database<Book> &, Log &) {
    system("rm login");
    exit(0);
}

op_su::op_su(const std::string &str) { input = str; }

void op_su::check(LoginStack &login, Database<User> &dtb, Database<Book> &, Log &) {
    TokenScanner scanner(input);
    count = scanner.getTokenCount();
    if (count != 2 && count != 3) error();
    scanner.nextToken();
    user = scanner.nextToken();
    int pvl_new = dtb.find(user.c_str()).Privilege;
    if (!pvl_new) error();
    if (count == 2) {
        int pvl = dtb.find(login.getUser().c_str()).Privilege;
        if (pvl <= pvl_new) error();
        return;
    }
    password = scanner.nextToken();
    ::check(user, "uop");
    ::check(password, "uop");
}

void op_su::execute(LoginStack &login, Database<User> &dtb, Database<Book> &, Log &log) {
    if (count == 2) {
        login.login(user);
        log.add_log("[info]" + user + " successfully login.");
    }
    else {
        if (password == dtb.find(user.c_str()).Password) {
            login.login(user);
            log.add_log("[info]" + user + " successfully login.");
        }
        else {
            error();
        }
    }
}

op_logout::op_logout(const std::string &str) { input = str; }

void op_logout::check(LoginStack &, Database<User> &, Database<Book> &, Log &) {
    TokenScanner scanner(input);
    if (scanner.getTokenCount() != 1) error();
}

void op_logout::execute(LoginStack &login, Database<User> &, Database<Book> &, Log &log) {
    std::string user = login.getUser();
    login.logout();
    log.add_log("[info]" + user + " logout.");
}

op_register::op_register(const std::string &str) { input = str; }

void op_register::check(LoginStack &, Database<User> &, Database<Book> &, Log &) {
    TokenScanner scanner(input);
    if (scanner.getTokenCount() != 4) error();
    scanner.nextToken();
    userID = scanner.nextToken();
    password = scanner.nextToken();
    username = scanner.nextToken();
    ::check(userID, "uop");
    ::check(password, "uop");
    ::check(username, "username");
}

void op_register::execute(LoginStack &login, Database<User> &dtb, Database<Book> &, Log &log) {
    if (dtb.find(userID.c_str()).Privilege) error();
    User newUser(userID, password, username, 1);
    dtb.insert(userID.c_str(), newUser);
    log.add_log("[info]" + userID + " registered by " + login.getUser() + ".");
}

op_passwd::op_passwd(const std::string &str) { input = str; }

void op_passwd::check(LoginStack &login, Database<User> &dtb, Database<Book> &, Log &) {
    if (!dtb.find(login.getUser().c_str()).Privilege) error();
    TokenScanner scanner(input);
    count = scanner.getTokenCount();
    if (count != 3 && count != 4) error();
    scanner.nextToken();
    user = scanner.nextToken();
    if (count == 3) {
        int pvl = dtb.find(login.getUser().c_str()).Privilege;
        if (pvl != 7) error();
        newPassword = scanner.nextToken();
        return;
    }
    currentPassword = scanner.nextToken();
    newPassword = scanner.nextToken();
    ::check(user, "uop");
    ::check(newPassword, "uop");
    ::check(currentPassword, "uop");
}

void op_passwd::execute(LoginStack &, Database<User> &dtb, Database<Book> &, Log &log) {
    User cur = dtb.find(user.c_str());
    if (!cur.Privilege) error();
    if (count == 3) {
        memset(cur.Password, 0, sizeof(cur.Password));
        for (int i = 0; i < newPassword.length(); ++i) {
            cur.Password[i] = newPassword[i];
        }
        dtb.modify(cur.UserID, cur);
        log.add_log("[info]" + user + " password changed by root.");
    }
    else {
        if (currentPassword == cur.Password) {
            memset(cur.Password, 0, sizeof(cur.Password));
            for (int i = 0; i < newPassword.length(); ++i) {
                cur.Password[i] = newPassword[i];
            }
            dtb.modify(cur.UserID, cur);
            log.add_log("[info]" + user + " password changed.");
        }
        else error();
    }
}

op_useradd::op_useradd(const std::string &str) { input = str; }

void op_useradd::check(LoginStack &login, Database<User> &dtb, Database<Book> &, Log &) {
    if (dtb.find(login.getUser().c_str()).Privilege < 3) error();
    TokenScanner scanner(input);
    if (scanner.getTokenCount() != 5) error();
    scanner.nextToken();
    userID = scanner.nextToken();
    password = scanner.nextToken();
    std::string pvl_temp = scanner.nextToken();
    username = scanner.nextToken();
    ::check(userID, "uop");
    ::check(password, "uop");
    ::check(pvl_temp, "pvl");
    ::check(username, "username");
    privilege = stoi(pvl_temp);
}

void op_useradd::execute(LoginStack &login, Database<User> &dtb, Database<Book> &, Log &log) {
    if (dtb.find(userID.c_str()).Privilege) error();
    int pvl = dtb.find(login.getUser().c_str()).Privilege;
    if (pvl <= privilege) error();
    User newUser(userID, password, username, privilege);
    dtb.insert(userID.c_str(), newUser);
    log.add_log("[info]" + login.getUser() + " added user " + userID + ".");
}

op_delete::op_delete(const std::string &str) { input = str; }

void op_delete::check(LoginStack &login, Database<User> &dtb, Database<Book> &, Log &) {
    if (dtb.find(login.getUser().c_str()).Privilege < 7) error();
    TokenScanner scanner(input);
    if (scanner.getTokenCount() != 2) error();
    scanner.nextToken();
    userID = scanner.nextToken();
    ::check(userID, "uop");
}

void op_delete::execute(LoginStack &login, Database<User> &dtb, Database<Book> &, Log &log) {
    if (login.checkLogin(userID)) error();
    if (!dtb.find(userID.c_str()).Privilege) error();
    dtb.erase(userID.c_str());
    log.add_log("[info]" + userID + " deleted by root.");
}

op_show::op_show(const std::string &str) { input = str; }

void op_show::check(LoginStack &login, Database<User> &dtb, Database<Book> &, Log &) {
    if (!dtb.find(login.getUser().c_str()).Privilege) error();
    TokenScanner scanner(input);
    count = scanner.getTokenCount();
    if (count != 1 && count != 2) error();
    if (count == 2) {
        scanner.nextToken();
        std::string buf = scanner.nextToken();
        para = checkPara(buf);
        val = checkVal(buf, para);
        ::check(val, para);
    }
}

void op_show::execute(LoginStack &login, Database<User> &, Database<Book> &dtb, Log &log) {
    if (count == 1) { dtb.show_books("all", "all"); }
    else { dtb.show_books(para, val); }
    log.add_log("[info]" + login.getUser() + " querying books.");
}

op_buy::op_buy(const std::string &str) { input = str; }

void op_buy::check(LoginStack &login, Database<User> &dtb, Database<Book> &, Log &) {
    if (!dtb.find(login.getUser().c_str()).Privilege) error();
    TokenScanner scanner(input);
    if (scanner.getTokenCount() != 3) error();
    scanner.nextToken();
    ISBN = scanner.nextToken();
    std::string Quantity_temp = scanner.nextToken();
    ::check(ISBN, "ISBN");
    ::check(Quantity_temp, "quantity");
    Quantity = std::stoi(Quantity_temp);
}

void op_buy::execute(LoginStack &login, Database<User> &, Database<Book> &dtb, Log &log) {
    if (Quantity <= 0) error();
    Book cur = dtb.find(ISBN.c_str());
    if (cur.stock < Quantity) error();
    cur.stock -= Quantity;
    dtb.modify(ISBN.c_str(), cur);
    std::cout << cur.Price * Quantity << std::endl;
    log.add_finance(cur.Price * Quantity);
    log.add_log("[info]" + login.getUser() + " bought books.");
    log.add_log("[finance] income + " + std::to_string(cur.Price * Quantity));
}

op_select::op_select(const std::string &str) { input = str; }

void op_select::check(LoginStack &login, Database<User> &dtb, Database<Book> &, Log &) {
    if (dtb.find(login.getUser().c_str()).Privilege < 3) error();
    TokenScanner scanner(input);
    if (scanner.getTokenCount() != 2) error();
    scanner.nextToken();
    ISBN = scanner.nextToken();
    ::check(ISBN, "ISBN");
}

void op_select::execute(LoginStack &login, Database<User> &, Database<Book> &dtb, Log &log) {
    Book cur = dtb.find(ISBN.c_str());
    if (cur.ISBN[0] == '\0') {
        Book newBook;
        for (int i = 0; i < ISBN.length(); ++i) {
            newBook.ISBN[i] = ISBN[i];
        }
        dtb.insert(newBook.ISBN, newBook);
        log.add_log("[info]book created.");
    }
    login.select(ISBN);
    log.add_log("[info]" + login.getUser() + " selected book.");
}

op_modify::op_modify(const std::string &str) { input = str; }

void op_modify::check(LoginStack &login, Database<User> &dtb, Database<Book> &, Log &) {
    if (dtb.find(login.getUser().c_str()).Privilege < 3) error();
}

void op_modify::execute(LoginStack &login, Database<User> &, Database<Book> &dtb, Log &log) {
    if (login.getISBN().empty()) error();
    Book cur = dtb.find(login.getISBN().c_str());
    std::string temp = cur.ISBN;
    bool table[5] = {false, false, false, false, false};
    TokenScanner scanner(input);
    scanner.nextToken();
    int count = scanner.getTokenCount();
    for (int i = 0; i < count - 1; ++i) {
        std::string buf = scanner.nextToken();
        std::string para = checkPara(buf);
        std::string val = checkVal(buf, para);
        ::check(val, para);
        if (para == "ISBN") {
            if (table[0]) error();
            //if (!strcmp(val.c_str(), cur.ISBN)) error();
            if (dtb.find(val.c_str()).ISBN[0] != '\0') error();
            memset(cur.ISBN, 0, sizeof(cur.ISBN));
            memmove(cur.ISBN, val.c_str(), val.length() * sizeof(char));
            //for (int j = 0; j < val.length(); ++j) { cur.ISBN[j] = val[j]; }
            table[0] = true;
        }
        else if (para == "name") {
            if (table[1]) error();
            memset(cur.BookName, 0, sizeof(cur.BookName));
            memmove(cur.BookName, val.c_str(), val.length() * sizeof(char));
            //for (int j = 0; j < val.length(); ++j) { cur.BookName[j] = val[j]; }
            table[1] = true;
        }
        else if (para == "author") {
            if (table[2]) error();
            memset(cur.Author, 0, sizeof(cur.Author));
            memmove(cur.Author, val.c_str(), val.length() * sizeof(char));
            //for (int j = 0; j < val.length(); ++j) { cur.Author[j] = val[j]; }
            table[2] = true;
        }
        else if (para == "keyword") {
            if (table[3]) error();
            memset(cur.Keyword, 0, sizeof(cur.Keyword));
            memmove(cur.Keyword, val.c_str(), val.length() * sizeof(char));
            //for (int j = 0; j < val.length(); ++j) { cur.Keyword[j] = val[j]; }
            table[3] = true;
        }
        else if (para == "price") {
            if (table[4]) error();
            cur.Price = std::stof(val);
            table[4] = true;
        }
        else error();
    }
    if (table[0]) {
        dtb.erase(temp.c_str());
        dtb.insert(cur.ISBN, cur);
        login.flush(temp, cur.ISBN);
    }
    else {
        dtb.modify(cur.ISBN, cur);
    }
    login.select(cur.ISBN);
    log.add_log("[info]" + login.getUser() + " modified book.");
}

op_import::op_import(const std::string &str) { input = str; }

void op_import::check(LoginStack &login, Database<User> &dtb, Database<Book> &, Log &) {
    if (dtb.find(login.getUser().c_str()).Privilege < 3) error();
    TokenScanner scanner(input);
    if (scanner.getTokenCount() != 3) error();
    scanner.nextToken();
    std::string Quantity_temp = scanner.nextToken();
    std::string TotalCost_temp = scanner.nextToken();
    ::check(Quantity_temp, "quantity");
    ::check(TotalCost_temp, "price");
    Quantity = std::stoi(Quantity_temp);
    TotalCost = std::stof(TotalCost_temp);
}

void op_import::execute(LoginStack &login, Database<User> &, Database<Book> &dtb, Log &log) {
    if (Quantity <= 0 || TotalCost <= 0) error();
    if (login.getISBN().empty()) error();
    Book cur = dtb.find(login.getISBN().c_str());
    cur.stock += Quantity;
    dtb.modify(login.getISBN().c_str(), cur);
    log.add_finance(-1 * TotalCost);
    log.add_log("[info]" + login.getUser() + " imported books.");
    log.add_log("[finance] spending - " + std::to_string(TotalCost));
}

op_show_finance::op_show_finance(const std::string &str) { input = str; }

void op_show_finance::check(LoginStack &login, Database<User> &dtb, Database<Book> &, Log &) {
    if (dtb.find(login.getUser().c_str()).Privilege < 7) error();
    TokenScanner scanner(input);
    count = scanner.getTokenCount();
    if (count != 2 && count != 3) error();
    if (count == 3) {
        scanner.nextToken();
        scanner.nextToken();
        std::string fcount_temp = scanner.nextToken();
        ::check(fcount_temp, "quantity");
        fcount = std::stoi(fcount_temp);
    }
    else {
        fcount = -1;
    }
}

void op_show_finance::execute(LoginStack &, Database<User> &, Database<Book> &, Log &log) {
    log.show_finance(fcount);
}

op_log::op_log(const std::string &str) { input = str; }

void op_log::check(LoginStack &, Database<User> &, Database<Book> &, Log &) {
    TokenScanner scanner(input);
    if (scanner.getTokenCount() != 1) error();
}

void op_log::execute(LoginStack &, Database<User> &, Database<Book> &, Log &log) {
    log.show_log();
}
