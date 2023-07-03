#include "menu.h"
#include "handle.h"
#include "DataSaveAndRead.h"
#include <set>
AdminMenu::AdminMenu(std::shared_ptr<Admin> admin_s) : 
    admin_ptr(admin_s) {}

static Data DataSave;

void AdminMenu::show()
{
    while (true)
    {
        std::cout << "=====Admin Menu=====\n";
        std::cout << "1.  Add Book\n";
        std::cout << "2.  Search Book\n";
        std::cout << "3.  Add Reader\n";
        std::cout << "4.  Search Reader\n";
        std::cout << "5.  Add Admin\n";
        std::cout << "6.  Search Admin\n";
        std::cout << "7.  Print Book List\n";
        std::cout << "8.  Print Reader List\n";
        std::cout << "9.  Print Admin List\n";
        std::cout << "10. Print Logs\n";
        std::cout << "11. Exit\n";

        std::cout << "Please enter your choice: ";

        int choice;
        handle::InputData(std::cin, "choice", choice, [](int choice){return choice >= 1 && choice <= 11;});
    
        system("cls");
        try{
            AdminMenu::run(choice);
        }
        catch (int)
        {
            std::cout << "Exit successfully!\n";
            break;
        }
        
        system("cls");
    }
}

void AdminMenu::run(int choice)
{
    switch (choice)
    {
    case 1:
        addBook();
        break;
    case 2:
        searchBook();
        break;
    case 3:
        addReader();
        break;
    case 4:
        searchReader();
        break;
    case 5:
        addAdmin();
        break;
    case 6:
        searchAdmin();
        break;
    case 7:
        printBookList();
        break;
    case 8:
        printReaderList();
        break;
    case 9:
        printAdminList();
        break;
    case 10:   
        printLogs();
        break;
    case 11:
        exit();
        break;
    default:
        break;
    }
    DataSave.Save();
    handle::Pause();
    return;
}

void AdminMenu::addBook ()
{
    if (admin_ptr->getLevel() < 3)
    {
        std::cout << admin_ptr->getName() + " don't have the permission!\n";
        return;
    }
    std::cout << "=====Add Book=====\n";

    std::cout << "Please enter the name of the book: ";
    std::string name_s;
    std::getline(std::cin, name_s);

    std::cout << "Please enter the ISBN of the book: ";
    std::string ISBN_s;
    std::getline(std::cin, ISBN_s);

    std::shared_ptr<Book> book_s = admin_ptr->addBook(name_s, ISBN_s);
    std::cout << "Add book successfully!\n";
   
    book_s->showBookInfo();
}

void AdminMenu::searchBook()
{
    if (admin_ptr->getLevel() < 2)
    {
        std::cout << admin_ptr->getName() + " don't have the permission!\n";
        return;
    }
    while (true)
    {
        system("cls");
        std::cout << "=====Search Book=====\n";
        std::cout << "1.  Search by Name\n";
        std::cout << "2.  Search by ISBN\n";
        std::cout << "3.  Search by ID\n";
        std::cout << "4.  Exit\n";
        std::cout << "Please enter your choice: ";
        int choice;

        handle::InputData(std::cin, "choice", choice, [](int choice){return choice >= 1 && choice <= 4;});

        std::vector<std::shared_ptr<Book>> book_list;

        std::string name_s;
        std::string ISBN_s;
        unsigned int ID_s;//不应该在switch内部定义变量
        
        switch (choice)
        {
        case 1:
            std::cout << "Please enter the name of the book: ";

            std::getline(std::cin, name_s);
            book_list = Book::searchBookByName(name_s);
            break;
        case 2:
            std::cout << "Please enter the ISBN of the book: ";

            std::getline(std::cin, ISBN_s);
            book_list = Book::searchBookByISBN(ISBN_s);
            break;
        case 3:
            std::cout << "Please enter the ID of the book: ";
            handle::InputData(std::cin, "ID", ID_s);
            book_list = Book::searchBookByID(ID_s);
            break;
        case 4:
            try{
                exit();
            }
            catch (int){
                std::cout << "Exit successfully!\n";
                return;
            }
        default:
            std::cerr << "Wrong input!\n";
            break;
        }

        system("cls");

        std::shared_ptr<Book> book_s;

        if (book_list.empty())
        {
            std::cout << "No book found!\n";
            handle::Pause();
            continue;
        }
        else if (book_list.size() == 1)
        {
            std::cout << "There is 1 book found!\n";
            book_s = book_list[0];
        }
        else
        {
            std::set<unsigned int> TempIdSet;
            std::cout << "There are " << book_list.size() << " books found!\n";
            for (auto &i : book_list){
                i->showBookInfo();
                TempIdSet.insert(i->getID());
                std::cout << std::endl;
            }

            std::cout << "Please enter the ID of the book: ";
            handle::InputData(std::cin, "ID", ID_s, [&](unsigned int ID){return TempIdSet.find(ID) != TempIdSet.end();} );//lambda表达式，引用传递，返回值为bool，参数为输入的值
            book_s = Book::getIDBookList()[ID_s];
        }

        system ("cls");
        book_s->showBookInfo();
        std::cout << std::endl;
        
        std::cout << "1. Edit Information\n";
        std::cout << "2. Delete Book\n";
        std::cout << "3. Exit\n";

        while (true)
        {
            std::cout << "Please enter your choice: ";
            handle::InputData(std::cin, "choice", choice, [](int choice){return choice >= 1 && choice <= 3;});

            if (choice <= 2 && book_s->getStatus() == 1)
            {
                std::cout << "This book is borrowed!\n";
                handle::Pause();
                continue;
            }

            bool flag = false;
            char ch_choice;

            switch (choice)
            {
            case 1:
                if(admin_ptr->getLevel() < 3)
                {
                    std::cout << admin_ptr->getName() + " don't have the permission!\n";
                    handle::Pause();
                    break;
                }
                std::cout << "=====Edit Information=====\n";
                std::cout << "Please enter the name of the book: ";
                std::getline(std::cin, name_s);

                std::cout << "Please enter the ISBN of the book: ";
                std::getline(std::cin, ISBN_s);

                admin_ptr->editBook(book_s, name_s, ISBN_s);
                std::cout << "Edit book successfully!\n";
                break;
            case 2:
                if(admin_ptr->getLevel() < 3)
                {
                    std::cout << admin_ptr->getName() + " don't have the permission!\n";
                    break;
                }
                std::cout << "Are you sure to delete this book?(y/n)\n";
                handle::InputData(std::cin, "choice", ch_choice, [](char choice){return choice == 'y' || choice == 'Y' || choice == 'n' || choice == 'N';});
                if (ch_choice == 'y' || ch_choice == 'Y'){
                    admin_ptr->deleteBook(book_s->getID());
                    std::cout << "Delete book successfully!\n";
                    flag = true;
                }
                break;
            case 3:
                std::cout << "Exit successfully!\n";
                flag = true;
                break;
            default:
                std::cerr << "Wrong input!\n";
                break;
            } 
            handle::Pause();

            if (flag)
                break;
        }
    }

    return;
}

void AdminMenu::searchAdmin()
{
    if (admin_ptr->getLevel() < 4)
    {
        std::cout << admin_ptr->getName() + " don't have the permission!\n";
        return;
    }

    while (true)
    {
        system("cls");
        std::cout << "=====Search Admin=====\n";
        std::cout << "1.  Search by Name\n";
        std::cout << "2.  Search by ID\n";
        std::cout << "3.  Exit\n";
        std::cout << "Please enter your choice: ";
        int choice;

        handle::InputData(std::cin, "choice", choice, [](int choice){return choice >= 1 && choice <= 3;});

        std::vector<std::shared_ptr<Admin>> admin_list;

        std::string name_s;
        std::string password_s;
        int level_s;
        unsigned int ID_s;

        switch (choice)
        {
        case 1:
            std::cout << "Please enter the name of the admin: ";

            std::getline(std::cin, name_s);
            admin_list = Admin::searchAdmin(name_s);
            break;
        case 2:
            std::cout << "Please enter the ID of the admin: ";
            handle::InputData(std::cin, "ID", ID_s);
            admin_list = Admin::searchAdmin(ID_s);
            break;
        case 3:
            try{
                exit();
            }
            catch (int){
                std::cout << "Exit successfully!\n";
                return;
            }
        default:
            std::cerr << "Wrong input!\n";
            return;
        }

        system("cls");

        std::shared_ptr<Admin> admin_s;

        if (admin_list.empty())
        {
            std::cout << "No admin found!\n";
            handle::Pause();
            continue;
        }
        else if (admin_list.size() == 1)
        {
            std::cout << "There is 1 admin found!\n";
            admin_s = admin_list[0];
        }
        else
        {
            std::set<unsigned int> TempIdSet;
            std::cout << "There are " << admin_list.size() << " admins found!\n";
            for (auto &i : admin_list){
                i->showInfo();
                TempIdSet.insert(i->getID());
                std::cout << std::endl;
            }
            std::cout << "Please enter the ID of the admin: ";
            handle::InputData(std::cin, "ID", ID_s, [&](unsigned int ID){return TempIdSet.find(ID) != TempIdSet.end();} );
            admin_s = Admin::getAdminList()[ID_s];
        }

        system("cls");
        admin_s->showInfo();
        std::cout << std::endl;

        std::cout << "1. Edit Information\n";
        std::cout << "2. Delete Admin\n";
        std::cout << "3. Exit\n";

        while (true)
        {
            std::cout << "Please enter your choice: ";
            handle::InputData(std::cin, "choice", choice, [](int choice){return choice >= 1 && choice <= 3;});

            char ch_choice;
            bool flag = false;
            switch (choice)
            {
            case 1:
                if(admin_ptr->getLevel() < 4)
                {
                    std::cout << admin_ptr->getName() + " don't have the permission!\n";
                    continue;
                }
                std::cout << "=====Edit Information=====\n";
                std::cout << "Please enter the name of the admin: ";
                std::getline(std::cin, name_s);
                
                std::cout << "Please enter the password of the admin: ";
                std::getline(std::cin, password_s);

                std::cout << "Please enter the level of the admin:(0~5)\n ";
                handle::InputData(std::cin, "level", level_s, [](int level){return level >= 0 && level <= 5;});

                admin_ptr->editAdmin(admin_s, name_s, password_s, level_s);
                std::cout << "Edit admin successfully!\n";
                break;
            case 2:
                if(admin_ptr->getLevel() < 4)
                {
                    std::cout << admin_ptr->getName() + " don't have the permission!\n";
                    break;
                }
                if (admin_ptr->getID() == admin_s->getID())
                {
                    std::cout << "You can't delete yourself!\n";
                    break;
                }
                std::cout << "Are you sure to delete this admin?(y/n)\n";
                handle::InputData(std::cin, "choice", ch_choice, [](char choice){return choice == 'y' || choice == 'Y' || choice == 'n' || choice == 'N';});
                if (ch_choice == 'y' || ch_choice == 'Y'){
                    admin_ptr->deleteAdmin(admin_s->getID());
                    std::cout << "Delete admin successfully!\n";
                    flag = true;
                }
                break;
            case 3:
                std::cout << "Exit successfully!\n";
                flag = true;
                break;
            default:
                std::cerr << "Wrong input!\n";
                break;
            }
            handle::Pause();
            if (flag)
                break;
        }
    }
    return;
}

void AdminMenu::searchReader()
{
    if (admin_ptr->getLevel() < 3)
    {
        std::cout << admin_ptr->getName() + " don't have the permission!\n";
        return;
    }
    while (true)
    {
        system("cls");
        std::cout << "=====Search Reader=====\n";
        std::cout << "1.  Search by Name\n";
        std::cout << "2.  Search by ID\n";
        std::cout << "3.  Exit\n";
        std::cout << "Please enter your choice: ";
        int choice;

        handle::InputData(std::cin, "choice", choice, [](int choice){return choice >= 1 && choice <= 3;});

        std::vector<std::shared_ptr<Reader>> reader_list;

        std::string name_s;
        std::string password_s;
        int level_s;
        unsigned int ID_s;//不应该在switch内部定义变量

        switch (choice)
        {
        case 1:
            std::cout << "Please enter the name of the reader: ";

            std::getline(std::cin, name_s);
            reader_list = Reader::searchReaderByName(name_s);
            break;
        case 2:
            std::cout << "Please enter the ID of the reader: ";
            handle::InputData(std::cin, "ID", ID_s);
            reader_list = Reader::searchReaderByID(ID_s);
            break;
        case 3:
            try{
                exit();
            }
            catch (int){
                std::cout << "Exit successfully!\n";
                return;
            }
        default:
            std::cerr << "Wrong input!\n";
            return;
        }

        system("cls");

        std::shared_ptr<Reader> reader_s;

        if (reader_list.empty())
        {
            std::cout << "No reader found!\n";
            handle::Pause();
            continue;
        }
        else if (reader_list.size() == 1)
        {
            reader_s = reader_list[0];
        }
        else
        {
            std::set<unsigned int> TempIdSet;
            std::cout << "There are " << reader_list.size() << " readers found!\n";
            for (auto &i : reader_list){
                i->showInfo();
                TempIdSet.insert(i->getID());
                std::cout << std::endl;
            }
            std::cout << "Please enter the ID of the reader: ";
            handle::InputData(std::cin, "ID", ID_s, [&](unsigned int ID){return TempIdSet.find(ID) != TempIdSet.end();} );
            reader_s = Reader::getReaderList()[ID_s];
        }

        system("cls");

        reader_s->showInfo();
        std::cout << "Book borrowed:\n";
        if (reader_s->getBookBorrow().empty())
            std::cout << "None!\n\n";
        else
            for (auto &i : reader_s->getBookBorrow())
                std::cout << i->getName() << std::endl;

        std::cout << std::endl;

        std::cout << "1. Edit Information\n";
        std::cout << "2. Delete Reader\n";
        std::cout << "3. Exit\n";

        while (true)
        {
            std::cout << "Please enter your choice: ";
            handle::InputData(std::cin, "choice", choice, [](int choice){return choice >= 1 && choice <= 3;});

            bool flag = false;
            char ch_choice;

            switch (choice)
            {
            case 1:
                if(admin_ptr->getLevel() < 3)
                {
                    std::cout << admin_ptr->getName() + " don't have the permission!\n";
                    break;
                }
                std::cout << "=====Edit Information=====\n";
                std::cout << "Please enter the name of the reader: ";
                std::getline(std::cin, name_s);
                
                std::cout << "Please enter the password of the reader: ";
                std::getline(std::cin, password_s);

                std::cout << "Please enter the level of the reader:(0~5)\n ";
                handle::InputData(std::cin, "level", level_s, [](int level){return level >= 0 && level <= 5;});

                admin_ptr->editReader(reader_s, name_s, password_s, level_s);
                std::cout << "Edit reader successfully!\n";
                break;
            case 2:
                if (!reader_s->getBookBorrow().empty())
                {
                std::cout << "This reader has borrowed books!\n";
                handle::Pause();
                continue;
                }

                if(admin_ptr->getLevel() < 3)
                {
                    std::cout << admin_ptr->getName() + " don't have the permission!\n";
                    break;
                }
                std::cout << "Are you sure to delete this reader?(y/n)\n";
                handle::InputData(std::cin, "choice", ch_choice, [](char choice){return choice == 'y' || choice == 'Y' || choice == 'n' || choice == 'N';});
                if (ch_choice == 'y' || ch_choice == 'Y'){
                    admin_ptr->deleteReader(reader_s->getID());
                    std::cout << "Delete reader successfully!\n";
                    flag = true;
                }
                break; 
            case 3:
                std::cout << "Exit successfully!\n";
                flag = true;
                break;
            default:
                std::cerr << "Wrong input!\n";
                break;
            }
            handle::Pause();
            if (flag)
                break;
        }
    }

    return;
}

void AdminMenu::addReader ()
{
    if (admin_ptr->getLevel() < 3)
    {
        std::cout << admin_ptr->getName() + " don't have the permission!\n";
        return;
    }
    std::cout << "=====Add Reader=====\n";
    std::cout << "Please enter the name of the reader: ";
    std::string name_s;
    std::getline(std::cin, name_s);

    std::cout << "Please enter the password of the reader: ";
    std::string password_s;
    std::getline(std::cin, password_s);

    std::shared_ptr<Reader> reader_s = admin_ptr->addReader(name_s, password_s);
    std::cout << "Add reader successfully!\n";

    reader_s->showInfo();
}

void AdminMenu::addAdmin()
{
    if (admin_ptr->getLevel() < 4)
    {
        std::cout << admin_ptr->getName() + " don't have the permission!\n";
        return;
    }
    std::cout << "=====Add Admin=====\n";
    std::cout << "Please enter the name of the admin: ";
    std::string name_s;
    std::getline(std::cin, name_s);

    std::cout << "Please enter the password of the admin: ";
    std::string password_s;
    std::getline(std::cin, password_s);

    int level_s;
    std::cout << "Please enter the level of the admin:(0~5,enter negative number to ignore this)\n ";
    std::cin >> level_s;
    try{
        handle::WrongInputCheck();
        if (level_s > 5)
            throw std::runtime_error("Wrong input!");
    }
    catch(const std::exception& e){
        handle::WrongInputHandle();
        handle::ClearBuffer();
        while (true)
        {
            std::cout << "Please enter the level again:\n";
            std::cin >> level_s;
            try{
                handle::WrongInputCheck();
            }
            catch(const std::exception& e){
                handle::WrongInputHandle();
                handle::ClearBuffer();
                continue;
            }
            break;
        }
    }
    handle::ClearBuffer();

    std::shared_ptr<Admin> admin_s = admin_ptr->addAdmin(name_s, password_s);
    if (level_s >= 0){
        admin_s->setLevel(level_s);
    }
    else{
        std::cout << "Level ignored!\n";
    }
    std::cout << "Add admin successfully!\n";

    admin_s->showInfo();

}

void AdminMenu::printBookList()
{
    admin_ptr->PrintBookList();
}

void AdminMenu::printReaderList()
{
    admin_ptr->PrintReaderList();
}

void AdminMenu::printAdminList()
{
    admin_ptr->PrintAdminList();
}

void AdminMenu::printLogs()
{
    admin_ptr->PrintLogs();
}

void AdminMenu::exit()
{
    DataSave.Save();
    throw 0;
}

ReaderMenu::ReaderMenu(std::shared_ptr<Reader> reader_s) : 
    reader_ptr(reader_s) {}

void ReaderMenu::show()
{
    while (true)
    {
        std::cout << "=====Reader Menu=====\n";
        std::cout << "1.  Borrow Book\n";
        std::cout << "2.  Return Book\n";
        std::cout << "3.  Print Borrowed Books\n";
        std::cout << "4.  Personal Info\n";
        std::cout << "5.  Exit\n";

        std::cout << "Please enter your choice: ";

        int choice;
        handle::InputData(std::cin, "choice", choice, [](int choice) {return choice <= 5 && choice >= 1;});
        
        system("cls");

        try{
            ReaderMenu::run(choice);
        }
        catch (int)
        {
            std::cout << "Exit successfully!\n";
            break;
        }

        handle::Pause();
        system("cls");
    }
}

void ReaderMenu::run(int choice)
{
    switch (choice)
    {
    case 1:
        borrowBook();
        break;
    case 2:
        returnBook();
        break;
    case 3:
        printBorrowedBooks();
        break;
    case 4:
        personalInfo();
        break;
    case 5:
        exit();
        break;
    default:
        std::cerr << "Wrong input!\n";
        break;
    }
    DataSave.Save();
    return;
}

void ReaderMenu::borrowBook()
{
    while (true){
        system("cls");
        std::cout << "=====Borrow Book=====\n";
        std::cout << "1.  Search by Name\n";
        std::cout << "2.  Search by ISBN\n";
        std::cout << "3.  Search by ID\n";
        std::cout << "4.  Exit\n";
        std::cout << "Please enter your choice: ";
        int choice;

        handle::InputData(std::cin, "choice", choice, [](int choice){return choice <= 4 && choice >= 1;} );

        std::vector<std::shared_ptr<Book>> book_list;

        std::string name_s;
        std::string ISBN_s;
        unsigned int ID_s;//不应该在switch内部定义变量
        
        switch (choice)
        {
        case 1:
            std::cout << "Please enter the name of the book: ";

            std::getline(std::cin, name_s);
            book_list = Book::searchBookByName(name_s);
            break;
        case 2:
            std::cout << "Please enter the ISBN of the book: ";

            std::cin >> ISBN_s;
            book_list = Book::searchBookByISBN(ISBN_s);
            break;
        case 3:
            std::cout << "Please enter the ID of the book: ";
            handle::InputData(std::cin, "ID", ID_s);
            book_list = Book::searchBookByID(ID_s);
            break;
        case 4:
            try{
                exit();
            }
            catch (int){
                std::cout << "Exit successfully!\n";
                return;
            }
        default:
            std::cerr << "Wrong input!\n";
            return;
        }

        if (book_list.empty())
        {
            std::cout << "No book found!\n";
            handle::Pause();
            continue;
        }
        else if (book_list.size() == 1)
        {
            book_list[0]->showBookInfo();
            ID_s = book_list[0]->getID();
        }
        else
        {
            std::set<unsigned int> exitSet;
            std::cout << "There are " << book_list.size() << " books found!\n";
            for (auto &i : book_list){
                i->showBookInfo();
                exitSet.insert(i->getID());
                std::cout << std::endl;
            }
            std::cout << "Please enter the ID of the book: ";
            handle::InputData(std::cin, "ID", ID_s, [&](unsigned int ID_s){return exitSet.count(ID_s) != 0;} );
        }

        std::cout << "Are you sure to borrow this book?(y/n)\n";
        char choicech;
        handle::InputData(std::cin, "choice", choicech, [](char choice){return choice == 'y' || choice == 'Y' || choice == 'n' || choice == 'N';});
        if (choicech != 'y' && choicech != 'Y')
            continue;

        reader_ptr->borrowBook(ID_s);
        DataSave.Save();
        handle::Pause();
    }
}

void ReaderMenu::returnBook()
{
    if (reader_ptr->getBookBorrow().empty())
    {
        std::cout << "You haven't borrowed any book!\n";
        return;
    }

    std::cout << "=====Return Book=====\n";

    std::cout << "The Books you have borrowed:\n";
    
    std::set<unsigned int> exitSet;

    auto borrowList = reader_ptr->getBookBorrow();

    for (auto i = borrowList.begin(); i != borrowList.end(); i++)
    {
        i->get()->showBookInfo();
        std::cout << "Borrowed time: " << reader_ptr->getTimeRecord()[i - borrowList.begin()] << std::endl;
        std::cout << std::endl;
        exitSet.insert((*i)->getID());
    }

    std::cout << "Please enter the ID of the book: ";
    unsigned int ID_s;
    handle::InputData(std::cin, "ID", ID_s, [&](unsigned int ID_s){return exitSet.count(ID_s) != 0;} );
    std::cout << "Are you sure to return this book?(y/n)\n";
    char choicech;
    handle::InputData(std::cin, "choice", choicech, [](char choice){return choice == 'y' || choice == 'Y' || choice == 'n' || choice == 'N';});
    if (choicech != 'y' && choicech != 'Y')
        return;
    reader_ptr->returnBook(ID_s);
}

void ReaderMenu::printBorrowedBooks()
{
    reader_ptr->PrintBorrowedBooks();
}

void ReaderMenu::personalInfo()
{
    std::cout << "=====Personal Info=====\n";
    std::cout << "Name: " << reader_ptr->getName() << std::endl;
    std::cout << "ID: " << reader_ptr->getID() << std::endl;
    std::cout << "Level: " << reader_ptr->getLevel() << std::endl;
    std::cout << "Capacity: " << reader_ptr->getCap() << std::endl;

    std::cout << "1. Edit Information\n";
    std::cout << "2. Exit\n";

    std::cout << "Please enter your choice: ";
    int choice;
    handle::InputData (std::cin, "choice", choice, [](int choice){return choice >= 1 && choice <= 2;});

    std::string name_s;
    std::string password_s;

    switch (choice)
    {
    case 1:
        std::cout << "=====Edit Information=====\n";
        std::cout << "Please enter the name of the reader: ";

        std::getline(std::cin, name_s);
        std::cout << "Please enter the password of the reader: ";
        std::getline(std::cin, password_s);

        reader_ptr->editInfo(name_s, password_s);
        std::cout << "Edit reader successfully!\n";

        DataSave.Save();
        break;
    case 2:
        std::cout << "Exit successfully!\n";
        break;
    default:
        std::cerr << "Wrong input!\n";
        break;
    }
}

void ReaderMenu::exit()
{
    DataSave.Save();
    throw 0;
}

void MainMenu::show()
{
    DataSave.Read();
    while (true)
    {
        std::cout << "=====Main Menu=====\n";
        std::cout << "1.  Login\n";
        std::cout << "2.  Exit\n";

        std::cout << "Please enter your choice: ";

        int choice;
        handle::InputData(std::cin, "choice", choice, [](int choice){return choice >= 1 && choice <= 2;});
        
        try{
            MainMenu::run(choice);
        }
        catch (int)
        {
            std::cout << "Exit successfully!\n";
            break;
        }
        handle::Pause();
        system("cls");
    }
}

void MainMenu::run(int choice)
{
    switch (choice)
    {
    case 1:
        system ("cls");
        login();
        break;
    case 2:
        system ("cls");
        exit();
        break;
    default:
        std::cerr << "Wrong input!\n";
        break;
    }
    DataSave.Save();
    return;
}

void MainMenu::login()
{
    while (true)
    {
        system ("cls");
        std::cout << "=====Login As=====\n";
        std::cout << "1.  Admin\n";
        std::cout << "2.  Reader\n";
        std::cout << "3.  Exit\n";
        std::cout << "Please enter your choice: ";

        int choice;
        handle::InputData(std::cin, "choice", choice, [](int choice){return choice >= 1 && choice <= 3;});
            
        switch (choice)
        {
        case 1:
            adminLogin();
            break;
        case 2:
            readerLogin();
            break;
        case 3:
            try{
                exit();
            }
            catch (int)
            {
                return;
            }
            break;
        default:
            std::cerr << "Wrong input!\n";
            break;
        }
        handle::Pause();
    }
}

void MainMenu::adminLogin ()
{
    system ("cls");
    std::cout << "=====Admin Login=====\n";
    std::cout << "Please enter your Name: ";
    std::string name_s;
    std::getline(std::cin, name_s);

    unsigned int ID_s;

    auto admin_list = Admin::searchAdmin(name_s);
    if (admin_list.empty()){
        std::cout << "No Admin named " << name_s << "!\n";
        return;
    }
    else if (admin_list.size() == 1){
        ID_s = admin_list[0]->getID();
    }
    else{
        std::set<unsigned int> TempIdSet;
        std::cout << "There are " << admin_list.size() << " admins named " << name_s << "!\n";
        for (auto &i : admin_list){
            i->showInfo();
            TempIdSet.insert(i->getID());
            std::cout << std::endl;
        }
        std::cout << "Please enter the ID of the admin: ";
        handle::InputData(std::cin, "ID", ID_s, [&](unsigned int ID){return TempIdSet.find(ID) != TempIdSet.end();});
        }

    std::cout << "Please enter your password: ";
    std::string password_s;
    std::getline(std::cin, password_s);

    std::shared_ptr<Admin> admin_s = Admin::Login(ID_s, password_s);
    if (admin_s == nullptr)
    {
        std::cout << "Login failed!\n";
        return;
    }
    std::cout << "Login successfully!\n";
    
    admin_s->showInfo();
    
    handle::AddLogs("Admin " + admin_s->getName() + "(ID: " + std::to_string(admin_s->getID()) + ")" +  "login at " + handle::getCurrentTime());

    std::cout << "Welcome!\n";
    
    handle::Pause();
    system("cls");

    AdminMenu admin_menu(admin_s);
    admin_menu.show();
}

void MainMenu::readerLogin ()
{
    system ("cls");
    std::cout << "=====Reader Login=====\n";
    std::cout << "Please enter your Name: ";
    std::string name_s;
    std::getline(std::cin, name_s);

    unsigned int ID_s;

    auto reader_list = Reader::searchReaderByName(name_s);
    if (reader_list.empty()){
        std::cout << "No Reader named " << name_s << "!\n";
        return;
    }
    else if (reader_list.size() == 1){
        ID_s = reader_list[0]->getID();
    }
    else{
        std::set<unsigned int> TempIdSet;
        std::cout << "There are " << reader_list.size() << " readers named " << name_s << "!\n";
        for (auto &i : reader_list){
            i->showInfo();
            TempIdSet.insert(i->getID());
            std::cout << std::endl;
        }
        std::cout << "Please enter the ID of the reader: ";
        handle::InputData(std::cin, "ID", ID_s, [&](unsigned int ID){return TempIdSet.find(ID) != TempIdSet.end();});
    }

    std::cout << "Please enter your password: ";
    std::string password_s;
    std::getline(std::cin, password_s);

    std::shared_ptr<Reader> reader_s = Reader::Login(ID_s, password_s);
    if (reader_s == nullptr)
    {
        std::cout << "Login failed!\n";
        return;
    }
    std::cout << "Login successfully!\n";

    reader_s->showInfo();

    handle::AddLogs("Reader " + reader_s->getName() + "(ID: " + std::to_string(reader_s->getID()) + ")" +  "login at " + handle::getCurrentTime());

    std::cout << "Welcome!\n";

    handle::Pause();

    system("cls");

    ReaderMenu reader_menu(reader_s);
    reader_menu.show();
}

void MainMenu::exit()
{
    DataSave.Save();
    throw 0;
}