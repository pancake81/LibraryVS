#include "Book.h"
#include "Reader.h"
#include "Library.h"
#include "UIConsole.h"

int main()
{
    try
    {
        Library& instance = Library::getInstance();

        while(true)
        {
            UIConsole::showMenu();
            UIConsole::Clause choice{ UIConsole::getRangedChoice(
                            UIConsole::ShowBooks,
                            UIConsole::Exit
                        ) };

            switch (choice)
            {
            case UIConsole::ShowBooks:
                UIConsole::showBooks(instance);
                break;
            case UIConsole::ShowReaders:
                UIConsole::showReaders(instance);
                break;
            case UIConsole::SearchBook:
                UIConsole::searchBook(instance);
                break;
            case UIConsole::SearchReader:
                UIConsole::searchReader(instance);
                break;
            case UIConsole::AddBook:
                UIConsole::addBook(instance);
                break;
            case UIConsole::AddReader:
                UIConsole::addReader(instance);
                break;
            case UIConsole::GiveBook:
                UIConsole::giveBook(instance);
                break;
            case UIConsole::GetBook:
                UIConsole::getBook(instance);
                break;
            case UIConsole::ShowReaderBooks:
                UIConsole::showReaderBooks(instance);
                break;
            case UIConsole::SaveChanges:
                UIConsole::saveChanges(instance);
                break;
            case UIConsole::Exit:
                return EXIT_SUCCESS;
            }
        }
    } catch (std::bad_alloc const&)
    {
        std::cout << "Not enough memory!" << std::endl;
        return EXIT_FAILURE;
    } catch (std::runtime_error const& err)
    /* All specific 'Library' class exceptions are inherited from std::runtime_error */
    {
        std::cout << err.what() << std::endl;
    }
}
