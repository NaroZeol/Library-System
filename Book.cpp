#include "Book.h"

Book::Book(const std::string &name_s, const std::string &ISBN_s, const unsigned int ID_s, const bool status_s):
    name(name_s), ISBN(ISBN_s), ID(ID_s), status(status_s){}

Book::Book(const Book &Book_s):
    name(Book_s.name), ISBN(Book_s.ISBN), ID(Book_s.ID), status(Book_s.status){}

Book::Book(const std::string &name_s, const std::string &ISBN_s):name(name_s), ISBN(ISBN_s)
{
    unsigned int ID_s = 0;
    while (Book::getIDBookList().find(ID_s) != Book::getIDBookList().end())
        ++ID_s;
    ID = ID_s;
    status = 0;
}

std::vector<std::shared_ptr<Book>> Book::searchBookByName (const std::string &name_s){
    auto &BookList = Book::getISBNBookList();
    std::vector<std::shared_ptr<Book>> temp;
    for (auto &i : BookList)
        if (i.second->getName() == name_s)
           temp.push_back(i.second);
    return temp;
}

std::vector<std::shared_ptr<Book>> Book::searchBookByISBN (const std::string &ISBN){
    auto &BookList = Book::getISBNBookList();
    auto range = BookList.equal_range(ISBN);
    
    if (range.first == range.second)
        return std::vector<std::shared_ptr<Book>>();

    std::vector<std::shared_ptr<Book>> temp;
    for (auto i = range.first; i != range.second; ++i)
        temp.push_back(i->second);
    return temp;
}

std::vector<std::shared_ptr<Book>> Book::searchBookByID (const unsigned int ID_s){
    auto &BookList = Book::getIDBookList();
    std::vector<std::shared_ptr<Book>> temp;
    if (BookList.find(ID_s) != BookList.end())
        temp.push_back(BookList[ID_s]);
    return temp;
}

std::multimap<std::string, std::shared_ptr<Book>> Book::BookISBNList;

std::map<unsigned int, std::shared_ptr<Book>> Book::BookIDList;

std::string Book::getName() const {return name;}

std::string Book::getISBN () const {return ISBN;}

unsigned int Book::getID () const {return ID;}

bool Book::getStatus () const {return status;}

void Book::setStatus (const bool status_s) {status = status_s;}

std::multimap<std::string, std::shared_ptr<Book>> &Book::getISBNBookList () {return BookISBNList;}

std::map<unsigned int, std::shared_ptr<Book>> &Book::getIDBookList () {return BookIDList;}

void Book::setName (const std::string &name_s) {name = name_s;}

void Book::setISBN (const std::string &ISBN_s) {ISBN = ISBN_s;}

void Book::setID (const unsigned int ID_s) {ID = ID_s;}

void Book::showBookInfo () const
{
    std::cout << "Name: " << name << std::endl;
    std::cout << "ID: " << ID << std::endl;
    std::cout << "ISBN: " << ISBN << std::endl;
    std::cout << "Status: " << (status ? "Borrowed" : "In Library") << std::endl;
}

