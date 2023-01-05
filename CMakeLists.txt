cmake_minimum_required(VERSION 3.16)
project(code)

set(CMAKE_CXX_STANDARD 14)

add_executable(code
        tokenScanner.hpp
        tokenScanner.cpp
        error.cpp
        error.hpp
        login.cpp
        login.hpp
        stock.cpp
        stock.hpp
        log.cpp
        log.hpp
        operation.cpp
        operation.hpp
        database.hpp
        main.cpp)
