#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <list>
#include <algorithm>
#include <fstream>

class Book
{
public:
// -- Типы --
    using IntType = std::size_t;
private:
// -- Поля --
    IntType _id;
    std::string _name;
    std::string _author;
    IntType _count;
// -- Методы --
public:
    Book(IntType = 0, std::string const& = "", std::string const& = "",  IntType = 0);
    IntType getId() const noexcept;
    IntType getCount() const noexcept;
    std::string getName() const;
    std::string getAuthor() const;
    void setCount(IntType) noexcept;

    friend std::fstream& operator>>(std::fstream&, Book&);
    friend std::fstream& operator<<(std::fstream&, Book const&);  // Почему Book& не const? См. реализацию
    friend bool compareNames(Book const&, Book const&) noexcept;
};

bool compareNames(Book const&, Book const&) noexcept;    // Повторное объявление, так как из вне не будет видно
