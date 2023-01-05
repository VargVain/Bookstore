#include "login.hpp"

User::User() {
    memset(UserID, 0, sizeof(UserID));
    memset(Password, 0, sizeof(Password));
    memset(Username, 0, sizeof(Username));
    Privilege = 0;
}

User::User(const std::string &UserID_, const std::string &Password_,
           const std::string &Username_, int Privilege_) {
    memset(UserID, 0, sizeof(UserID));
    memset(Password, 0, sizeof(Password));
    memset(Username, 0, sizeof(Username));
    for (int i = 0; i < UserID_.length(); ++i) { UserID[i] = UserID_[i]; }
    for (int i = 0; i < Password_.length(); ++i) { Password[i] = Password_[i]; }
    for (int i = 0; i < Username_.length(); ++i) { Username[i] = Username_[i]; }
    Privilege = Privilege_;
}

LoginStack::LoginStack() {
    file.open("login");
    if (!file) {
        file.open("login", std::ios::out | std::ios::binary);
    }
    usersCount = 0;
    file.close();
}

LoginStack::~LoginStack() = default;

std::string LoginStack::getUser() {
    if (usersCount == 0) return "";
    loginUser cur;
    file.open("login", std::ios::binary | std::ios::in | std::ios::out);
    file.seekg((usersCount - 1) * sizeof(loginUser), std::ios::beg);
    file.read((char *)(&cur), sizeof(loginUser));
    file.close();
    std::string ret(cur.UserID);
    return ret;
}

std::string LoginStack::getISBN() {
    if (usersCount == 0) return "";
    loginUser cur;
    file.open("login", std::ios::binary | std::ios::in | std::ios::out);
    file.seekg((usersCount - 1) * sizeof(loginUser), std::ios::beg);
    file.read((char *)(&cur), sizeof(loginUser));
    file.close();
    std::string ret(cur.ISBN);
    return ret;
}

void LoginStack::login(const std::string &UserID) {
    loginUser cur;
    for (int i = 0; i < UserID.length(); ++i) {
        cur.UserID[i] = UserID[i];
    }
    file.open("login", std::ios::binary | std::ios::in | std::ios::out);
    file.seekp(usersCount * sizeof(loginUser), std::ios::beg);
    file.write((char *)(&cur), sizeof(loginUser));
    file.close();
    ++usersCount;
}

void LoginStack::select(const std::string &ISBN) {
    if (usersCount == 0) return;
    loginUser cur;
    file.open("login", std::ios::binary | std::ios::in | std::ios::out);
    file.seekg((usersCount - 1) * sizeof(loginUser), std::ios::beg);
    file.read((char *)(&cur), sizeof(loginUser));
    for (int i = 0; i < ISBN.length(); ++i) {
        cur.ISBN[i] = ISBN[i];
    }
    file.seekp((usersCount - 1) * sizeof(loginUser), std::ios::beg);
    file.write((char *)(&cur), sizeof(loginUser));
    file.close();
}

void LoginStack::logout() {
    if (usersCount == 0) error();
    --usersCount;
}

bool LoginStack::checkLogin(const std::string &UserID) {
    int pos = 0;
    loginUser cur;
    while (pos < usersCount) {
        file.seekg(pos * sizeof(loginUser));
        file.read((char *)(&cur), sizeof(loginUser));
        if (UserID == cur.UserID) return true;
        ++pos;
    }
    return false;
}
