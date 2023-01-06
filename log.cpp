#include "log.hpp"

Log::Log() {
    file.open("log");
    if (!file) {
        file.open("log", std::ios::out | std::ios::binary);
        int count = 0;
        file.seekp(0);
        file.write((char *)(&count), sizeof(int));
        file.seekp(sizeof(int));
        file.write((char *)(&count), sizeof(int));
    }
    file.close();
    file.open("my_log");
    if (!file) {
        file.open("my_log", std::ios::out | std::ios::binary);
        int count = 0;
        file.seekp(0);
        file.write((char *)(&count), sizeof(int));
    }
    file.close();
}

int Log::get_count() {
    int count;
    file.open("log", std::ios::binary | std::ios::in | std::ios::out);
    file.seekg(0, std::ios::beg);
    file.read((char *)(&count), sizeof(int));
    file.close();
    return count;
}

int Log::get_log_count() {
    int count;
    file.open("my_log", std::ios::binary | std::ios::in | std::ios::out);
    file.seekg(0, std::ios::beg);
    file.read((char *)(&count), sizeof(int));
    file.close();
    return count;
}

void Log::add_finance(double val) {
    int count = get_count() + 1;
    file.open("log", std::ios::binary | std::ios::in | std::ios::out);
    file.seekp(count * sizeof(double), std::ios::beg);
    file.write((char *)(&val), sizeof(double));
    file.close();
    file.open("log", std::ios::binary | std::ios::in | std::ios::out);
    file.seekp(0, std::ios::beg);
    file.write((char *)(&count), sizeof(int));
    file.close();
}

void Log::show_finance(int count) {
    int tcount = get_count();
    if (count > tcount) error();
    else if (count == 0) {
        std::cout << '\n';
    }
    else if (count == -1) {
        double pos = 0;
        double neg = 0;
        for (int i = 0; i < tcount; ++i) {
            double buf;
            file.open("log", std::ios::binary | std::ios::in | std::ios::out);
            file.seekg((i + 1) * sizeof(double), std::ios::beg);
            file.read((char *)(&buf), sizeof(double));
            file.close();
            if (buf > 0) pos += buf;
            else neg -= buf;
        }
        std::cout << "+ " << pos << " - " << neg << '\n';
    }
    else {
        double pos = 0;
        double neg = 0;
        file.open("log", std::ios::binary | std::ios::in | std::ios::out);
        file.seekg((tcount - count + 1) * sizeof(double));
        for (int i = 0; i < count; ++i) {
            double buf;
            file.read((char *)(&buf), sizeof(double));
            if (buf > 0) pos += buf;
            else neg -= buf;
        }
        file.close();
        std::cout << "+ " << pos << " - " << neg << '\n';
    }
}

void Log::add_log(const std::string &str) {
    char log[64];
    memset(log, 0, sizeof(log));
    for (int i = 0; i < str.length(); ++i) {
        log[i] = str[i];
    }
    int count = get_log_count();
    file.open("my_log", std::ios::binary | std::ios::in | std::ios::out);
    file.seekp(64 * count * sizeof(char) + sizeof(int), std::ios::beg);
    file.write((char *)(&log), sizeof(log));
    file.close();
    ++count;
    file.open("my_log", std::ios::binary | std::ios::in | std::ios::out);
    file.seekp(0, std::ios::beg);
    file.write((char *)(&count), sizeof(int));
    file.close();
}

void Log::show_log() {
    int count = get_log_count();
    for (int i = 0; i < count; ++i) {
        char buf[64];
        file.open("my_log", std::ios::binary | std::ios::in | std::ios::out);
        file.seekg(64 * i * sizeof(char) + sizeof(int), std::ios::beg);
        file.read((char *)(&buf), sizeof(buf));
        file.close();
        std::cout << std::string(buf) << '\n';
    }
}

