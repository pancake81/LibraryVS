#include "Book.h"

Book::Book(IntType id, std::string const& name, std::string const& author, IntType count):
    _id{ id }, _name{ name }, _author{ author }, _count{ count }
{}

Book::IntType Book::getId() const noexcept
{
    return _id;
}

Book::IntType Book::getCount() const noexcept
{
    return _count;
}

std::string Book::getName() const
{
    return _name;
}

std::string Book::getAuthor() const
{
    return _author;
}

void Book::setCount(IntType count) noexcept
{
    _count = count;
}

std::fstream& operator>>(std::fstream& stream, Book& book)
{
    stream >> book._id >> book._name >> book._author >> book._count;
    return stream;
}

std::fstream& operator<<(std::fstream& stream, Book const& book)
{
    static_cast<std::ostream&>(stream) << book._id << '\t' << book._name << '\t' << book._author << '\t' << book._count;
    return stream;
}

bool compareNames(Book const& lh,Book const& rh) noexcept
{
    return lh._name < rh._name;
}
