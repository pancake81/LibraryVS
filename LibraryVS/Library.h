#pragma once
#include "Book.h"
#include "Reader.h"
#include <set>
#include <map>
#include <vector>
#include <algorithm>

class Library           // Singleton class
{
    /* Next 5 properties will be getting from lib.conf */
    Book::IntType _nextBookID;          // Autoincriment books ID 
    Reader::IntType _nextReaderID;      // Autoincriment readers ID 
    std::string _booksFileName;         // Books list file name
    std::string _readersFileName;       // Readers list file name
    std::string _rbFileName;            // 'Reader->book' table file name

    std::multiset<Book, decltype(&compareNames)> booksCache{compareNames};          // Books collection
    std::set<Reader, decltype(&comparePassports)> readersCache{comparePassports};   // Readers collection
    std::multimap<Reader, Book, decltype(&compareIDs)> rbCache{compareIDs};         // 'Reader->book' table

    std::fstream _booksFile;
    std::fstream _readersFile;
    std::fstream _rbFile;
    std::fstream _confFile;

    Library();          // Can throw specific exceptions (see below)
    Library(Library const&) = delete;
    Library& operator=(Library const&) const = delete;

    auto searchBookID(Book::IntType) const noexcept;  // Helper searching function

    /* For loading lists in cache */
    void readBooksFile();
    void readReadersFile();
    void readRBFile();
    void readConfFile();

    /* For writing lists in files */
    bool writeBooksFile();
    bool writeReadersFile();
    bool writeRBFile();
    bool writeConfFile();
public:
    class FilesOpenning : public std::runtime_error
    {
    public:
        FilesOpenning(std::string const& msg) : std::runtime_error(msg) {}
    };
    class FilesIOOperations : public std::runtime_error
    {
    public:
        FilesIOOperations(std::string const& msg) : std::runtime_error(msg) {}
    };
    class ConfigError : public std::runtime_error
    {
    public:
        ConfigError(std::string const& msg) : std::runtime_error(msg) {}
    };

    enum Error {OK, NoBooks, BookGived, NoRBRecords};  // Status errors which returned from rents books methods

    ~Library();
    static Library& getInstance();

    /* Library's functional */
    const std::multiset<Book, decltype(&compareNames)>& getAllBooks() const noexcept;
    const std::set<Reader, decltype(&comparePassports)>& getAllReaders() const noexcept;
    std::vector<Book> getReaderBooks(Reader const&) const;
    std::vector<Book> searchBook(Book const&) const;
    Reader searchReader(Reader const&) const;
    std::vector<Book> approxSearchBook(Book const&) const;       // Books approximate search
    std::vector<Reader> approxSearchReader(Reader const&) const; // Readers approximate search
    void addBook(Book const&);
    bool addReader(Reader const&);
    Error giveBookForReader(Reader const&, Book const&);
    Error getBookFromReader(Reader const&, Book const&);
    Book::IntType getBookIDInc() noexcept;                      // Return ID for new book
    Reader::IntType getReaderIDInc() noexcept;                  // Return ID for new reader
    bool saveChanges();      
};
