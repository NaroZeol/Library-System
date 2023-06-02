#ifndef BOOK_H_
#define BOOK_H_
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <memory>
class Book{
private:
    std::string name;
    std::string ISBN;//ISBN一般都很长，所以用string
    unsigned int ID;
    static std::multimap<std::string, std::shared_ptr<Book>> BookISBNList;//记录ISBN,当要使用一类书进行操作时选择这个，因为ISBN可能重复，所以用multimap
    static std::map<unsigned int, std::shared_ptr<Book>> BookIDList;//记录独立ID，当要使用单独一本书进行操作时选择这个
    bool status;//0:在库 1:借出
public:
    Book () = default;
    Book (const Book &Book_s);
    Book (const std::string &name_s, const std::string &ISBN_s, const unsigned int ID_s, const bool status_s);
    Book (const std::string &name_s, const std::string &);//自动生成ID，默认为在库
    ~Book () {};

    std::string getName() const;
    std::string getISBN () const;
    unsigned int getID () const;
    bool getStatus () const;

    void setName (const std::string &name_s);
    void setISBN (const std::string &ISBN_s);
    void setID (const unsigned int ID_s);
    void setStatus (const bool status_s);

    void showBookInfo () const;

    static std::vector<std::shared_ptr<Book>> searchBookByName (const std::string &name_s);
    static std::vector<std::shared_ptr<Book>> searchBookByISBN (const std::string &ISBN);
    static std::vector<std::shared_ptr<Book>> searchBookByID (const unsigned int ID);

    static std::multimap<std::string, std::shared_ptr<Book>> &getISBNBookList ();
    static std::map<unsigned int, std::shared_ptr<Book>> &getIDBookList ();
};



#endif