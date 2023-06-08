#ifndef MENU_H_
#define MENU_H_
#include "Person.h"
#include "Book.h"

class MenuBase
{
public:
    MenuBase() = default;
    virtual ~MenuBase() = default;
    virtual void show() = 0;
    virtual void run(int choice) = 0;
};

class AdminMenu : public MenuBase
{
private:
    void addBook();
    void searchBook();
    void addReader();
    void searchReader();
    void addAdmin();
    void searchAdmin();
    void printBookList();
    void printReaderList();
    void printAdminList();
    void printLogs();
    void exit();
    std::shared_ptr<Admin> admin_ptr;
public:
    AdminMenu() = default;
    AdminMenu(std::shared_ptr<Admin> admin_s);
    ~AdminMenu() = default;
    void show() override;
    void run(int choice) override;
};

class ReaderMenu : public MenuBase
{
private:
    void borrowBook();
    void returnBook();
    void printBorrowedBooks();
    void personalInfo();
    void exit();
    std::shared_ptr<Reader> reader_ptr;
public:
    ReaderMenu() = default;
    ReaderMenu(std::shared_ptr<Reader> reader_s);
    ~ReaderMenu() = default;
    void show() override;
    void run(int choice) override;
};

class MainMenu : public MenuBase
{
private:
    void login();
    void adminLogin();
    void readerLogin();
    void exit();
public:
    MainMenu() = default;
    ~MainMenu() = default;
    void show() override;
    void run(int choice) override;
};
#endif