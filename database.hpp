#ifndef BOOKSTORE_DATABASE_HPP
#define BOOKSTORE_DATABASE_HPP

#include <memory.h>
#include <set>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "tokenScanner.hpp"
#include "error.hpp"

template <class T>
class Database {
public:
    Database() = delete;

    explicit Database(const std::string &str);

    void insert(const char *_key, T _val);

    void erase(const char *_key);

    void modify(const char *_key, T _val);

    T find(const char *_key);

    void show_books(const std::string &para, const std::string &val);

private:
    static const int kMaxSize = 10;
    static const int keyLen = 32;
    struct Data {
        char key[keyLen];
        T val;
        Data() : val(T()) { memset(key, 0, keyLen); }
        Data(const char * _key, T _value) : val(_value) {
            memset(key, 0, keyLen);
            for (int i = 0; i < strlen(_key); ++i) { key[i] = _key[i]; }
        }
        ~Data() = default;
        Data& operator=(const Data &rhs) {
            if (&rhs == this) return *this;
            else {
                memset(key, 0, sizeof(key));
                for (int i = 0; i < strlen(rhs.key); ++i) { key[i] = rhs.key[i]; }
                val = rhs.val;
            }
            return *this;
        }
        friend bool operator<(const Data &lhs, const Data &rhs) {
            return strcmp(lhs.key, rhs.key) < 0;
        }
        friend bool operator>(const Data &lhs, const Data &rhs) {
            return strcmp(lhs.key, rhs.key) > 0;
        }
    };
    struct Block {
        Data max;
        int next_dest;
        int dest;
        int size;
        int kansimeiyong = 0;
        Data data[kMaxSize];
        Block() : dest(0), next_dest(-1), size(0) {}
        ~Block() = default;
    };
    struct Head {
        int blocks_count = 1;
        int deleted[32];
        Head() : blocks_count(1) {}
    };

    void getBlock(Block &cur, int dest);

    void writeBlock(const Block &cur, int dest);

    //0-next_dest, 1-dest, 2-size.
    int getInt(int dest, int bios);

    Data getData(int dest, int bios);

    void writeInt(int val, int dest, int bios);

    void writeData(Data val, int dest, int bios);

    int CreateBlock(int prev, int next);

    int locate(const char *_key);

    std::fstream file;
    std::string filename;
};

template<class T>
Database<T>::Database(const std::string &str) {
    filename = str;
    file.open(filename);
    if (!file) {
        file.open(filename, std::ios::out | std::ios::binary);
        Head head;
        Block block;
        file.write((char *)(&head), sizeof(Head));
        file.write((char *)(&block), sizeof(Block));
    }
    file.close();
}

template<class T>
void Database<T>::insert(const char *_key, T _val) {
    Data _data(_key, _val);
    int pos = locate(_key);
    Data cur_max = getData(pos, -1);
    int cur_size = getInt(pos, 2);
    if (cur_size == kMaxSize - 1) {
        Block cur;
        Block newBlock;
        getBlock(cur, pos);
        int pos_new = CreateBlock(cur.dest, cur.next_dest);
        cur.data[cur.size++] = _data;
        std::sort(cur.data, cur.data + kMaxSize);
        cur.size = kMaxSize >> 1;
        newBlock.next_dest = getInt(pos_new, 0);
        newBlock.dest = pos_new;
        newBlock.size = kMaxSize >> 1;
        newBlock.max = cur.max;
        cur.next_dest = pos_new;
        cur.max = cur.data[(kMaxSize >> 1) - 1];
        memmove(newBlock.data, cur.data + (kMaxSize >> 1), (kMaxSize >> 1) * sizeof(Data));
        writeBlock(cur, pos);
        writeBlock(newBlock, pos_new);
    }
    else {
        cur_max = strcmp(cur_max.key, _key) > 0 ? cur_max : _data;
        writeData(cur_max, pos, -1);
        writeData(_data, pos, cur_size);
        writeInt(++cur_size, pos, 2);
    }
}

template<class T>
void Database<T>::erase(const char *_key) {
    int pos = locate(_key);
    Block cur;
    getBlock(cur, pos);
    for (int i = 0; i < cur.size; ++i) {
        if (!strcmp(cur.data[i].key, _key)) {
            cur.data[i] = cur.data[cur.size - 1];
            --cur.size;
            if (cur.size == 0) {
                if (pos != 0) {
                    int dest_temp = 0;
                    int dest_before = 0;
                    while (dest_temp != pos) {
                        dest_before = dest_temp;
                        dest_temp = getInt(dest_temp, 0);
                    }
                    writeInt(cur.next_dest, dest_before, 0);
                    return;
                }
            }
            else {
                if (_key == cur.max.key) {
                    int max_temp = 0;
                    for (int j = 1; j < cur.size; ++j) {
                        max_temp = cur.data[max_temp] > cur.data[j] ? max_temp : j;
                    }
                    writeData(cur.data[max_temp], pos, -1);
                }
            }
            writeData(cur.data[i], pos, i);
            writeInt(cur.size, pos, 2);
            return;
        }
    }
}

template<class T>
void Database<T>::modify(const char *_key, T _val) {
    int pos = locate(_key);
    Data _data(_key, _val);
    Block cur;
    getBlock(cur, pos);
    for (int i = 0; i < cur.size; ++i) {
        if (!strcmp(cur.data[i].key, _key)) {
            writeData(_data, pos, i);
            return;
        }
    }
}

template<class T>
T Database<T>::find(const char *_key) {
    int pos = locate(_key);
    Block cur;
    getBlock(cur, pos);
    for (int i = 0; i < cur.size; ++i) {
        if (!strcmp(cur.data[i].key, _key)) return cur.data[i].val;
    }
    return T();
}

template<class T>
void Database<T>::getBlock(Block &cur, int dest) {
    file.open(filename, std::ios::binary | std::ios::in | std::ios::out);
    file.seekg(dest * sizeof(Block) + sizeof(Head), std::ios::beg);
    file.read((char *)(&cur), sizeof(Block));
    file.close();
}

template<class T>
void Database<T>::writeBlock(const Block &cur, int dest) {
    file.open(filename, std::ios::binary | std::ios::in | std::ios::out);
    file.seekp(dest * sizeof(Block) + sizeof(Head), std::ios::beg);
    file.write((char *)(&cur), sizeof(Block));
    file.close();
}

template<class T>
int Database<T>::getInt(int dest, int bios) {
    int ret;
    file.open(filename, std::ios::binary | std::ios::in | std::ios::out);
    file.seekg(dest * sizeof(Block) + sizeof(Head) + sizeof(Data) + bios * sizeof(int), std::ios::beg);
    file.read((char *)(&ret), sizeof(int));
    file.close();
    return ret;
}

template<class T>
typename Database<T>::Data Database<T>::getData(int dest, int bios) {
    Data ret;
    file.open(filename, std::ios::binary | std::ios::in | std::ios::out);
    if (bios == -1) {
        file.seekg(dest * sizeof(Block) + sizeof(Head), std::ios::beg);
    }
    else {
        file.seekg(dest * sizeof(Block) + sizeof(Head) + (bios + 1) * sizeof(Data) + 4 * sizeof(int), std::ios::beg);
    }
    file.read((char *)(&ret), sizeof(Data));
    file.close();
    return ret;
}

template<class T>
void Database<T>::writeInt(int val, int dest, int bios) {
    file.open(filename, std::ios::binary | std::ios::in | std::ios::out);
    file.seekp(dest * sizeof(Block) + sizeof(Head) + sizeof(Data) + bios * sizeof(int), std::ios::beg);
    file.write((char *)(&val), sizeof(int));
    file.close();
}

template<class T>
void Database<T>::writeData(Data val, int dest, int bios) {
    Data ret;
    file.open(filename, std::ios::binary | std::ios::in | std::ios::out);
    if (bios == -1) {
        file.seekp(dest * sizeof(Block) + sizeof(Head), std::ios::beg);
    }
    else {
        file.seekp(dest * sizeof(Block) + sizeof(Head) + (bios + 1) * sizeof(Data) + 4 * sizeof(int), std::ios::beg);
    }
    file.write((char *)(&val), sizeof(Data));
    file.close();
}

template<class T>
int Database<T>::CreateBlock(int prev, int next) {
    file.open(filename, std::ios::binary | std::ios::in | std::ios::out);
    file.seekg(0, std::ios::beg);
    file.seekp(0, std::ios::beg);
    Head curHead;
    file.read((char *)(&curHead), sizeof(Head));
    file.close();
    Block temp, before;
    getBlock(before, prev);
    temp.next_dest = next;
    temp.dest = curHead.blocks_count;
    ++curHead.blocks_count;
    before.next_dest = temp.dest;
    writeBlock(temp, temp.dest);
    writeBlock(before, prev);
    file.open(filename, std::ios::binary | std::ios::in | std::ios::out);
    file.seekp(0, std::ios::beg);
    file.write((char *)(&curHead), sizeof(Head));
    file.close();
    return temp.dest;
}

template<class T>
int Database<T>::locate(const char *_key) {
    file.open(filename, std::ios::binary | std::ios::in | std::ios::out);
    Data max;
    int dest = 0;
    file.seekg(sizeof(Head), std::ios::beg);
    file.read((char *)(&max), sizeof(Data));
    file.read((char *)(&dest), sizeof(int));
    while (strcmp(max.key, _key) < 0 && dest != -1) {
        file.seekg(dest * sizeof(Block) + sizeof(Head), std::ios::beg);
        file.read((char *)(&max), sizeof(Data));
        file.read((char *)(&dest), sizeof(int));
    }
    file.read((char *)(&dest), sizeof(int));
    file.close();
    return dest;
}

template<class T>
void Database<T>::show_books(const std::string &para, const std::string &val) {
    bool flag = false;
    if (para == "keyword") {
        for (int i = 0; i < val.length(); ++i) {
            if (val[i] == '|') error();
        }
    }
    int dest = 0;
    int cur_size = getInt(dest, 2);
    Data _data;
    while (dest != -1) {
        std::set<Data> ret;
        if (para == "all") {
            for (int i = 0; i < cur_size; ++i) {
                Data it = getData(dest, i);
                ret.insert(it);
            }
        }
        else {
            if (para == "ISBN") {
                for (int i = 0; i < cur_size; ++i) {
                    _data = getData(dest, i);
                    if (val == _data.val.ISBN) ret.insert(_data);
                }
            }
            else if (para == "name") {
                for (int i = 0; i < cur_size; ++i) {
                    _data = getData(dest, i);
                    if (val == _data.val.BookName) ret.insert(_data);
                }
            }
            else if (para == "author") {
                for (int i = 0; i < cur_size; ++i) {
                    _data = getData(dest, i);
                    if (val == _data.val.Author) ret.insert(_data);
                }
            }
            else if (para == "keyword") {
                for (int i = 0; i < cur_size; ++i) {
                    _data = getData(dest, i);
                    if (consist(val, _data.val.Keyword)) ret.insert(_data);
                }
            }
            else error();
        }
        if (!ret.empty()) {
            for (auto it = ret.begin(); it != ret.end(); ++it) {
                std::cout << it->val.ISBN << '\t' << it->val.BookName <<
                          '\t' << it->val.Author << '\t' << it->val.Keyword <<
                          '\t' << it->val.Price << '\t' << it->val.stock << '\n';
            }
            flag = true;
        }
        dest = getInt(dest, 0);
        if (!flag) std::cout << '\n';
    }
}

#endif //BOOKSTORE_DATABASE_HPP
