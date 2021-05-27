#include "UIConsole.h"
#include <cstdlib>
#include <climits>

std::string UIConsole::replaceOnSpace(std::string const& str)   // Possible throw bad_alloc
{
    std::string ret(str.size(), ' ');
    std::transform(str.cbegin(), str.cend(), ret.begin(), [](auto const& ch) {
        if (ch == '^') return ' ';
        else return ch;
    });
   
    return ret;
}

std::string UIConsole::replaceOnSpec(std::string const& str)    // Possible throw bad_alloc
{
    std::string ret(str.size(), ' ');
    std::transform(str.cbegin(), str.cend(), ret.begin(), [](auto const& ch) {
        if (ch == ' ') return '^';
        else return ch;
    });

    return ret;
}

bool UIConsole::hasSmth(std::string const& str)
{
    return std::find_if_not(str.begin(), str.end(), ::isspace) != str.end();
}

std::string UIConsole::trim(std::string const& src)
{
    auto begin_ns{ std::find_if_not(src.begin(), src.end(), ::isspace) };
    auto end_ns{ src.end() };
    if (begin_ns != src.end()) {
        end_ns = std::find_if_not(src.rbegin(), src.rend(), ::isspace).base();
    }
    return std::string(begin_ns, end_ns);
}

size_t UIConsole::getChoice()   // Possible throw bad_alloc
{
    size_t choice{};
    while(!(std::cin >> choice))
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Wrong choice! Please, repeat> ";
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');   // For occasion like '13xxxx' input (clean xxxx)
    
    return choice;
}

// Possible throw bad_alloc
size_t UIConsole::getRanged(std::size_t min, std::size_t max)
{
    size_t ret{ getChoice() };
    while (!(min <= ret && ret <= max))
    {
        std::cout << "Wrong clause! Try again> ";
        ret = getChoice();
    }
    return ret;
}

// Possible throw bad_alloc
UIConsole::Clause UIConsole::getRangedChoice(UIConsole::Clause min, UIConsole::Clause max)
{
    UIConsole::Clause ret{ static_cast<UIConsole::Clause>(getChoice()) };
    while(!(min <= ret && ret <= max))
    {
        std::cout << "Wrong clause! Try again> ";
        ret = static_cast<UIConsole::Clause>(getChoice());
    }
    return ret;
}

void UIConsole::showMenu() noexcept
{
    system("CLS");
    std::cout << "\tChoose necessery clause:\n"
              "1. Show all books in the library.\n"
              "2. Show all readers in the library.\n"
              "3. Search book.\n"
              "4. Search reader.\n"
              "5. Add new book.\n"
              "6. Add new reader.\n"
              "7. Give book to reader.\n"
              "8. Get book from reader.\n"
              "9. Show reader's books.\n"
              "10. Save changes.\n"
              "11. Exit.\n"
                 "> ";
}

void UIConsole::showBooks(Library const& instance)
{
    try
    {
        showBooksOrReaders<Book>(instance.getAllBooks());
    } catch (std::bad_alloc const&)
    {
        std::cout << "Not enough memory!" << std::endl;
    }

    system("pause");
}

void UIConsole::showReaders(Library const& instance)
{
    try
    {
        showBooksOrReaders<Reader>(instance.getAllReaders());
    } catch (std::bad_alloc const&)
    {
        std::cout << "Not enough memory!" << std::endl;
    }

    system("pause");
}

void UIConsole::searchBook(Library const& instance)
{
    try
    {
        std::string name, forSkip;
        do
        {
            std::cout << "Enter book name> ";
            getline(std::cin, name);
        } while(!hasSmth(name));

        auto result{ instance.searchBook(Book(0, replaceOnSpec(trim(name)))) };
        if(!result.empty())
            showBooksOrReaders<Book>(result);
        else
        {
            std::cout << "Nothing is found. Do you want to start aproximate search (y/n)?> ";
            char answer = 'n'; std::cin >> answer;
            if(answer == 'y' || answer == 'Y')
            {
                std::getline(std::cin, forSkip);  // Clean excess characters
                showBooksOrReaders<Book>(instance.approxSearchBook(Book(0, replaceOnSpec(trim(name)))));
            }
            else
                std::getline(std::cin, forSkip);  // Clean excess characters
        }
    } catch (std::bad_alloc const&)
    {
        std::cout << "Not enough memory!" << std::endl;
    }

    system("pause");
}

void UIConsole::searchReader(Library const& instance)
{
    try
    {
        std::string passport, fio, forSkip;
        do
        {
            std::cout << "Enter reader's passport> ";
            getline(std::cin, passport);
        } while(!hasSmth(passport));

        auto reader{ instance.searchReader(Reader(0, "", replaceOnSpec(trim(passport)))) };
        if(reader.getId() == 0)
        {
            std::cout << "Nothing is found. Do you want to start aproximate search (y/n)?> ";
            char answer = 'n'; std::cin >> answer;
            if(answer == 'y' || answer == 'Y')
            {
                std::getline(std::cin, forSkip);   // Clean excess characters
                do
                {
                    std::cout << "Enter reader's name> ";
                    getline(std::cin, fio);
                } while(!hasSmth(fio));

                showBooksOrReaders<Reader>(instance.approxSearchReader(Reader(0, replaceOnSpec(trim(fio)))));
            }
            else
                std::getline(std::cin, forSkip);   // Clean excess characters
        }
        else
            std::cout << reader << std::endl;
    } catch (std::bad_alloc const&)
    {
        std::cout << "Not enough memory!" << std::endl;
    }

    system("pause");
}

void UIConsole::addBook(Library & instance)
{
    try
    {
        std::string name, author;

        do
        {
            std::cout << "Enter book name> ";
            getline(std::cin, name);
        } while(!hasSmth(name));
        do
        {
            std::cout << "Enter author> ";
            getline(std::cin, author);
        } while(!hasSmth(author));
        std::cout << "Enter count of books> ";
        auto count{ getChoice() };

        instance.addBook(Book(instance.getBookIDInc(),
                              UIConsole::replaceOnSpec(trim(name)),
                              UIConsole::replaceOnSpec(trim(author)),
                              count));
    } catch (std::bad_alloc const&)
    {
        std::cout << "Not enough memory!" << std::endl;
    }

    system("pause");
}

void UIConsole::addReader(Library & instance)
{
    try
    {
        std::string fio, passport;

        do
        {
            std::cout << "Enter reader name> ";
            getline(std::cin, fio);
        } while (!hasSmth(fio));
        do
        {
            std::cout << "Enter passport> ";
            getline(std::cin, passport);
        } while(!hasSmth(fio));

        if(instance.addReader(Reader(instance.getReaderIDInc(),
                              UIConsole::replaceOnSpec(trim(fio)),
                              UIConsole::replaceOnSpec(trim(passport)))))
            std::cout << "OK." << std::endl;
        else
            std::cout << "Error. This reader already exist." << std::endl;
    } catch (std::bad_alloc const&)
    {
        std::cout << "Not enough memory!" << std::endl;
    }

    system("pause");
}

void UIConsole::giveBook(Library& instance)
{
    try
    {
        std::string bookName;
        std::string readerPass;
        Book book;

        do // Searching reader.
        {
            std::cout << "Enter reader's passport> ";
            getline(std::cin, readerPass);
        } while (!hasSmth(readerPass));
        auto reader{ instance.searchReader(Reader(0, "", replaceOnSpec(trim(readerPass)))) };
        if (reader.getId() == 0)
        {
            std::cout << "Reader isn't found." << std::endl;
            system("pause");
            return;
        }
        do
        {
            std::cout << "Enter book name that you want to give> ";
            getline(std::cin, bookName);
        } while (!hasSmth(bookName));
        auto books{ instance.searchBook(Book(0, replaceOnSpec(trim(bookName)))) };    // Get books list which can contain one.
        if (books.empty())   // Stop if nothing found
        {
            std::string forSkip;
            std::cout << "Nothing is found. Do you want to start aproximate search (y/n)?> ";
            char answer = 'n'; std::cin >> answer;
            if (answer == 'y' || answer == 'Y')
            {
                std::getline(std::cin, forSkip);  // Clean excess characters
                books = instance.approxSearchBook(Book(0, replaceOnSpec(trim(bookName))));
                if (books.empty())
                {
                    std::cout << "Book isn't found." << std::endl;
                    system("pause");
                    return;
                }
            }
            else
            {
                std::getline(std::cin, forSkip);  // Clean excess characters
                system("pause");
                return;
            }
        }
        if (books.size() > 1)   // If there are several books, we require to chose concrete book.
        {
            showBooksOrReaders<Book>(books);
            std::cout << "There is several books. Choose particular book [enter 1 - " << books.size() << "]: ";
            auto choice{ getRanged(1, books.size()) };
            book = books[choice - 1];
        }
        else
            book = books[0];

        Library::Error what{ instance.giveBookForReader(reader, book) };
        if (what == Library::OK)
        {
            std::cout << "OK. " << std::endl;
            system("pause");
        }
        else if (what == Library::BookGived)
        {
            std::cout << "This book is already gived." << std::endl;
            system("pause");
        }
        else if (what == Library::NoBooks)
        {
            std::cout << "No more this book in the library." << std::endl;
            system("pause");
        }
    }
    catch (std::bad_alloc const&)
    {
        std::cout << "Not enough memory!" << std::endl;
        system("pause");
    }
}

void UIConsole::getBook(Library & instance)
{
    try
    {
        std::string bookName;
        std::string readerPass;
        Book book;

        do // Searching reader
        {
            std::cout << "Enter reader's passport> ";
            getline(std::cin, readerPass);
        } while(!hasSmth(readerPass));
        auto reader{ instance.searchReader(Reader(0, "", replaceOnSpec(trim(readerPass)))) };
        if(reader.getId() == 0)
        {
            std::cout << "Reader isn't found." << std::endl;
            system("pause");
            return;
        }
        do
        {
            std::cout << "Enter book name that you want to get> ";
            getline(std::cin, bookName);
        } while(!hasSmth(bookName));
        auto books{ instance.searchBook(Book(0, replaceOnSpec(trim(bookName)))) };    // Get books list which can contain one.
        if(books.empty())   // Stop if nothing found
        {
            std::string forSkip;
            std::cout << "Nothing is found. Do you want to start aproximate search (y/n)?> ";
            char answer = 'n'; std::cin >> answer;
            if (answer == 'y' || answer == 'Y')
            {
                std::getline(std::cin, forSkip);  // Clean excess characters
                books = instance.approxSearchBook(Book(0, replaceOnSpec(trim(bookName))));
                if (books.empty())
                {
                    std::cout << "Book isn't found." << std::endl;
                    system("pause");
                    return;
                }
            }
            else
            {
                std::getline(std::cin, forSkip);  // Clean excess characters
                system("pause");
                return;
            }
        }
        if(books.size() > 1)   // If there are several books, we require to chose concrete book.
        {
            showBooksOrReaders<Book>(books);
            std::cout << "There are several books. Choose particular book [enter 1 - " << books.size() << "]: ";
            auto choice{ getRanged(1, books.size()) };
            book = books[choice - 1];
        }
        else
            book = books[0];

        Library::Error what{ instance.getBookFromReader(reader, book) }; // Checking status
        if(what == Library::NoRBRecords)
        {
            std::cout << "This reader never gets book." << std::endl;
            system("pause");
            return;
        }
        else if(what == Library::NoBooks)
        {
            std::cout << "This book wasn't gived to this reader." << std::endl;
            system("pause");
            return;
        }
        else
        {
            std::cout << "OK." << std::endl;
            system("pause");
        }
    } catch (std::bad_alloc const&)
    {
        std::cout << "Not enough memory!" << std::endl;
        system("pause");
    }
}

void UIConsole::showReaderBooks(Library const& instance)
{
    try
    {
        std::string passport;

        std::cout << "Enter reader's passport> ";
        getline(std::cin, passport);
        auto reader{ instance.searchReader(Reader(0, "", replaceOnSpec(trim(passport)))) };
        if(reader.getId() == 0)
        {
            std::cout << "This reader doesn't exist." << std::endl;
            system("pause");
            return;
        }

        auto books{ instance.getReaderBooks(reader) };
        if(books.empty())
        {
            std::cout << "This reader doesn't have any books." << std::endl;
            system("pause");
            return;
        }
        else
        {
            showBooksOrReaders<Book>(books);
            system("pause");
        }
    } catch (std::bad_alloc const&)
    {
        std::cout << "Not enough memory!" << std::endl;
        system("pause");
    }

}

void UIConsole::saveChanges(Library& lib)
{
    if (lib.saveChanges())
        std::cout << "Ok." << std::endl;
    
    system("pause");
}

std::ostream& operator << (std::ostream& stream, Book const& book)  // Possible throw bad_alloc
{
    stream << std::setw(10) << book.getId() << std::setw(25) << UIConsole::replaceOnSpace(book.getName())
           << std::setw(25) << UIConsole::replaceOnSpace(book.getAuthor()) << std::setw(10) << book.getCount();
    return stream;
}

std::ostream& operator << (std::ostream& stream, Reader const& reader)  // Possible throw bad_alloc
{
    stream << std::setw(10) << reader.getId() << std::setw(25) << UIConsole::replaceOnSpace(reader.getFio())
           << std::setw(15) << UIConsole::replaceOnSpace(reader.getPassport());
    return stream;
}
