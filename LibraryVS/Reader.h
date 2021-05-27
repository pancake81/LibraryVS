#pragma once

#include<string>
#include<iostream>
#include<sstream>
#include<list>
#include<algorithm>
#include <fstream>

class Reader
{
public:
// -- Типы --
    using IntType = size_t;
private:
// -- Поля --
    IntType _id;
    std::string _fio;
    std::string _passport;
// -- Методы --
public:
    Reader(IntType = 0, std::string const& = "", std::string const& = "");
    IntType getId() const noexcept;
    std::string getFio() const;
    std::string getPassport() const;

    friend std::fstream& operator>>(std::fstream&, Reader&);
    friend std::fstream& operator<<(std::fstream&, Reader const&);
    friend bool compareIDs(Reader const&, Reader const&) noexcept;
    friend bool comparePassports(Reader const&, Reader const&) noexcept;
};

bool compareFios(Reader const&, Reader const&) noexcept;
bool compareIDs(Reader const&, Reader const&) noexcept;
bool comparePassports(Reader const&, Reader const&) noexcept;
