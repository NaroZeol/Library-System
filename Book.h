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
    std::string ISBN;//ISBNһ�㶼�ܳ���������string
    unsigned int ID;
    static std::multimap<std::string, std::shared_ptr<Book>> BookISBNList;//��¼ISBN,��Ҫʹ��һ������в���ʱѡ���������ΪISBN�����ظ���������multimap
    static std::map<unsigned int, std::shared_ptr<Book>> BookIDList;//��¼����ID����Ҫʹ�õ���һ������в���ʱѡ�����
    bool status;//0:�ڿ� 1:���
public:
    Book () = default;
    Book (const Book &Book_s);
    Book (const std::string &name_s, const std::string &ISBN_s, const unsigned int ID_s, const bool status_s);
    Book (const std::string &name_s, const std::string &);//�Զ�����ID��Ĭ��Ϊ�ڿ�
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