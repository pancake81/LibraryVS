#include "Library.h"

Library::Library()
{
    _confFile.open("lib.conf", std::ios_base::in | std::ios_base::out | std::ios_base::binary);
    if(!_confFile.is_open())     // If opening config file failed, no sense to continue.
        throw FilesOpenning("Fail opening lib.conf file.");
    readConfFile();     // Throw exception if reading errors occurred.

    _booksFile.open(_booksFileName, std::ios_base::in | std::ios_base::out | std::ios_base::binary);
    if (!_booksFile.is_open()) // If opening file failed, try to create new file.
    {
        _booksFile.clear();
        _booksFile.open(_booksFileName, std::ios_base::out);
        _booksFile.close();
        _booksFile.open(_booksFileName, std::ios_base::in | std::ios_base::out | std::ios_base::binary);
        if (!_booksFile.is_open())
            throw FilesOpenning("Fail opening " + _booksFileName + " file.");
    }

    _readersFile.open(_readersFileName, std::ios_base::in | std::ios_base::out | std::ios_base::binary);
    if (!_readersFile.is_open())
    {
        _readersFile.clear();
        _readersFile.open(_readersFileName, std::ios_base::out);
        _readersFile.close();
        _readersFile.open(_readersFileName, std::ios_base::in | std::ios_base::out | std::ios_base::binary);
        if (!_readersFile.is_open())
            throw FilesOpenning("Fail opening " + _readersFileName + " file.");
    }

    _rbFile.open(_rbFileName, std::ios_base::in | std::ios_base::out | std::ios_base::binary);
    if (!_rbFile.is_open())
    {
        _rbFile.clear();
        _rbFile.open(_rbFileName, std::ios_base::out);
        _rbFile.close();
        _rbFile.open(_rbFileName, std::ios_base::in | std::ios_base::out | std::ios_base::binary);
        if (!_rbFile.is_open())
            throw FilesOpenning("Fail opening " + _rbFileName + " file.");
    }

    /* Methods possible throw exceptions */
    readBooksFile();
    readReadersFile();
    readRBFile();
}

Library::~Library()
{
    saveChanges();

    _booksFile.close();
    _readersFile.close();
    _rbFile.close();
    _confFile.close();
}

Library& Library::getInstance()
{
    static Library _instance;
    return _instance;
}

const std::multiset<Book, decltype(&compareNames)>& Library::getAllBooks() const noexcept
{
    return booksCache;
}

const std::set<Reader, decltype(&comparePassports)>& Library::getAllReaders() const noexcept
{
    return readersCache;
}

std::vector<Book> Library::getReaderBooks(Reader const& reader) const   // Possible throw bad_alloc
{
    std::vector<Book> booksList;

    if(!rbCache.empty())
    {
        auto books{ rbCache.equal_range(reader) };
        for (auto it{ books.first }; it != books.second; ++it)
            booksList.push_back(it->second);
    }

    return booksList;
}

std::vector<Book> Library::searchBook(Book const& book) const   // Possible throw bad_alloc
{
    std::vector<Book> booksList;

    if(!booksCache.empty())
    {
        auto findedBooks{ booksCache.equal_range(book) };
        if(findedBooks.first != booksCache.end())
        {
            std::for_each(findedBooks.first, findedBooks.second, [&booksList](const Book& book){
                booksList.push_back(book);
            });
        }
    }

    return booksList;
}

Reader Library::searchReader(Reader const& reader) const    // Possible throw bad_alloc
{
    if(!readersCache.empty())
    {
        auto itReader{ readersCache.find(reader) };
        if(itReader != readersCache.end())
            return *itReader;
    }

    return Reader();
}

std::vector<Book> Library::approxSearchBook(Book const& book) const // Possible throw bad_alloc
{
    auto lowerStr1{ book.getName() };
    std::vector<Book> ret;

    // Approximate search strings
    for(std::size_t i = 0; i < lowerStr1.length(); ++i)
        lowerStr1[i] = static_cast<char>(std::tolower(lowerStr1[i]));

    for(const auto& item : booksCache)
    {
        auto lowerStr2{ item.getName() };

        for(std::size_t i = 0; i < lowerStr2.length(); ++i)
            lowerStr2[i] = static_cast<char>(std::tolower(lowerStr2[i]));

        if(lowerStr2.find(lowerStr1) != std::string::npos)
            ret.push_back(item);
    }

    return ret;
}

std::vector<Reader> Library::approxSearchReader(Reader const& reader) const // Possible throw bad_alloc
{
    // Searching by name
    auto lowerStr1{ reader.getFio() };
    std::vector<Reader> ret;

    // Approximate search strings
    for(std::size_t i = 0; i < lowerStr1.length(); ++i)
        lowerStr1[i] = static_cast<char>(std::tolower(lowerStr1[i]));

    for(const auto& item : readersCache)
    {
        auto lowerStr2{ item.getFio() };

        for(std::size_t i = 0; i < lowerStr2.length(); ++i)
            lowerStr2[i] = static_cast<char>(std::tolower(lowerStr2[i]));

        if(lowerStr2.find(lowerStr1) != std::string::npos)
            ret.push_back(item);
    }

    return ret;
}

void Library::addBook(Book const& book) // Possible throw bad_alloc
{
    if(!booksCache.empty())             // Try to find the book we want to insert
    {
        auto findedBooks{ booksCache.equal_range(book) };
        bool inserted{};
        if(findedBooks.first != booksCache.end())
        {
            for (auto it{ findedBooks.first }; it != findedBooks.second; ++it)
                if(it->getAuthor() == book.getAuthor())   // If the book's authors are the same, we increase the count of books
                {
                    Book b = *it;
                    b.setCount(it->getCount() + book.getCount());
                    booksCache.erase(it);
                    booksCache.insert(b);
                    inserted = true;
                    break;
                }
            if(!inserted)   // Insert the book if it isn't found
                booksCache.insert(book);
        }
        else
        {
            booksCache.insert(book);
            return;
        }
    }
    else
    {
        booksCache.insert(book);
        return;
    }
}


bool Library::addReader(Reader const& reader)   // Possible throw bad_alloc
{
    return readersCache.insert(reader).second;
}

auto Library::searchBookID(Book::IntType id) const noexcept
{
    for (auto it{ booksCache.begin() }; it != booksCache.end(); ++it)
        if(it->getId() == id)
            return it;

    return booksCache.end();
}

Library::Error Library::giveBookForReader(Reader const& reader, Book const& book)   // Possible throw bad_alloc
{
    auto itBook{ searchBookID(book.getId()) };

    if(itBook->getCount() == 0)  // Stop if there are no more books
        return Library::NoBooks;
    auto booksForReader{ rbCache.equal_range(reader) };  // Stop if the reader already have the book
    for(auto it = booksForReader.first; it != booksForReader.second; ++it)
    {
        if(it->second.getId() == itBook->getId())
            return Library::BookGived;
    }

    /* Updating books count */
    auto b{ *itBook };
    b.setCount(itBook->getCount() - 1);
    booksCache.erase(itBook);
    booksCache.insert(b);

    rbCache.insert({reader, b});

    return Library::OK;
}

Library::Error Library::getBookFromReader(Reader const& reader, Book const& book)   // Possible throw bad_alloc
{
    auto booksForReader{ rbCache.equal_range(reader) };
    if(booksForReader.first == rbCache.end())   // Checking the reader rented the book ever.
        return Library::NoRBRecords;

    for (auto it{ booksForReader.first }; it != booksForReader.second; ++it)
    {
        if(it->second.getId() == book.getId())  // If the reader rented this book, we take it from him and increase the books count
        {
            auto itBook{ searchBookID(book.getId()) };
            Book b = *itBook;
            if(itBook == booksCache.end())
                b.setCount(1);
            else
            {
                b.setCount(itBook->getCount() + 1);
                booksCache.erase(itBook);
            }

            booksCache.insert(b);
            rbCache.erase(it);
            return Library::OK;
        }
    }

    return Library::NoBooks;
}

Book::IntType Library::getBookIDInc() noexcept
{
    return _nextBookID++;
}

Reader::IntType Library::getReaderIDInc() noexcept
{
    return _nextReaderID++;
}

void Library::readBooksFile()
{
    try
    {
        Book book;

        while(_booksFile >> book)
            booksCache.insert(book);
        if(!_booksFile.eof())
        {
            throw FilesIOOperations("Reading error " + _booksFileName + " file.");
        }
    } catch (std::bad_alloc const&)
    {
        throw FilesIOOperations("Reading error " + _booksFileName + " file.");
    }
}

void Library::readReadersFile()
{
    try
    {
        Reader reader;

        while(_readersFile >> reader)
            readersCache.insert(reader);
        if(!_readersFile.eof())
        {
            throw FilesIOOperations("Reading error " + _readersFileName + " file.");
        }
    } catch (std::bad_alloc const&)
    {
        throw FilesIOOperations("Reading error " + _readersFileName + " file.");
    }
}

void Library::readRBFile()
{
    try
    {
        Reader reader;
        Book book;

        while(_rbFile >> reader >> book)
            rbCache.insert({reader, book});
        if(!_rbFile.eof())
        {
            throw FilesIOOperations("Reading error " + _rbFileName + " file.");
        }
    } catch (std::bad_alloc const&)
    {
        throw FilesIOOperations("Reading error " + _rbFileName + " file.");
    }

}

void Library::readConfFile()
{
    try
    {
        std::string justForSkip;

        if(!(_confFile >> justForSkip >> _nextBookID &&
            _confFile >> justForSkip >> _nextReaderID &&
            _confFile >> justForSkip >> _booksFileName &&
            _confFile >> justForSkip >> _readersFileName &&
            _confFile >> justForSkip >> _rbFileName))
        {
            throw FilesIOOperations("Fail reading lib.conf file.");
        }
        else
        {
            if(_nextBookID == 0 || _nextReaderID == 0)
                throw ConfigError("Incorrect properties. IDs cannot be equal to 0.");
        }
    } catch (std::bad_alloc const&)
    {
        throw FilesIOOperations("Fail reading lib.conf file.");
    }
}

bool Library::writeBooksFile()
{
    _booksFile.close();
    _booksFile.open(_booksFileName, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
    if(!_booksFile.is_open())
        return false;

    for(auto const& item : booksCache)
        if(!(_booksFile << item << std::endl))
            return false;

    _booksFile.flush();
    return true;
}

bool Library::writeReadersFile()
{
    _readersFile.close();
    _readersFile.open(_readersFileName, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
    if(!_readersFile.is_open())
        return false;

    for(auto const& item : readersCache)
        if(!(_readersFile << item << std::endl))
            return false;

    _readersFile.flush();
    return true;
}

bool Library::writeRBFile()
{
    _rbFile.close();
    _rbFile.open(_rbFileName, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
    if(!_rbFile.is_open())
        return false;

    for(auto const& item : rbCache)
        if(!(static_cast<std::fstream&>(_rbFile << item.first << '\t') << item.second << std::endl))
            return false;

    _rbFile.flush();
    return true;
}

bool Library::writeConfFile()
{
    _confFile.close();
    _confFile.open("lib.conf", std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
    if(!_confFile.is_open())
        return  false;

    if(!(_confFile << "booksIDstart:\t" << _nextBookID << std::endl &&
        _confFile << "readersIDstart:\t" << _nextReaderID << std::endl &&
        _confFile << "booksFile:\t" << _booksFileName << std::endl &&
        _confFile << "readersFile:\t" << _readersFileName << std::endl &&
        _confFile << "rbFile:\t" << _rbFileName << std::endl)
        )
            return false;

    _confFile.flush();
    return true;
}

bool Library::saveChanges()
{
    bool ret{ true };
    if (!writeBooksFile())
    {
        std::cout << "Error saving " << _booksFileName << std::endl;
        ret = false;
    }
    if (!writeReadersFile())
    {
        std::cout << "Error saving " << _readersFileName << std::endl;
        ret = false;
    }
    if (!writeRBFile())
    {
        std::cout << "Error saving " << _rbFileName << std::endl;
        ret = false;
    }
    if (!writeConfFile())
    {
        std::cout << "Error saving lib.conf! You should make properties again(" << std::endl;
        ret = false;
    }

    return ret;
}
