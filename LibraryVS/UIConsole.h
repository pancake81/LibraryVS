#pragma once
#include <string>
#include <iostream>
#include <typeinfo>
#include <iomanip>
#include "Book.h"
#include "Reader.h"
#include "Library.h"

class UIConsole
{
public:
    enum Clause               // Menu's clauses
    {ShowBooks = 1, ShowReaders, SearchBook, SearchReader,
     AddBook, AddReader, GiveBook, GetBook, ShowReaderBooks, SaveChanges, Exit};

    /*
     * Because of reading until whitespace character by operator >> we substitute '^'
     * instead of whitespace when writing info.
    */
    static std::string replaceOnSpace(std::string const&);
    static std::string replaceOnSpec(std::string const&);

    /* For validating string input */
    static bool hasSmth(std::string const&);
    static std::string trim(std::string const&);

    static size_t getChoice();                          // For treating numbers input
    static Clause getRangedChoice(Clause, Clause);      // For validating menu's clause
    static size_t getRanged(std::size_t, std::size_t);  // For validating some list's clause

    static void showMenu() noexcept;             // Clean console and show menu

    /* UI class's wrapper functions for Library class functional */
    static void showBooks(Library const&);
    static void showReaders(Library const&);
    static void searchBook(Library const&);
    static void searchReader(Library const&);
    static void addBook(Library&);
    static void addReader(Library&);
    static void giveBook(Library&);
    static void getBook(Library&);
    static void showReaderBooks(Library const&);
    static void saveChanges(Library&);

    /* Template function for books\readers lists output */
    template<typename Entity, typename Container>
    static void showBooksOrReaders(Container const& container)
    {
        if(!container.empty())
        {
            if(typeid (Entity) == typeid (Reader))
                std::cout << std::setw(10) << "ID|" << std::setw(25) << "Name|" << std::setw(15) <<  "Passport|" << std::endl;
            if(typeid (Entity) == typeid (Book))
                std::cout << std::setw(10) << "ID|" << std::setw(25) << "Name|" << std::setw(25) << "Author|" << std::setw(10) << "Count|" << std::endl;
             
            std::for_each(container.cbegin(), container.cend(), [](Entity const& item){
                std::cout << item << std::endl;
            });
        }
        else
            std::cout << "Nothing is found." << std::endl;
    }

};

/* Helper operators for book/reader output */
std::ostream& operator << (std::ostream&, Book const&);
std::ostream& operator << (std::ostream&, Reader const&);
