#ifndef PERSON_H_
#define PERSON_H_

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include "Book.h"

class Person{
    friend class Data;
protected:
    std::string name;
    std::string password;
    unsigned int ID;
    int level;
public:
    Person () = default;
    Person (const std::string &name_s, const std::string &password_s, unsigned int ID_s, int level);
    ~Person() = default;

    std::string getName() const;
    void setName(const std::string &str);

    int getLevel()const;
    void setLevel(const int level_s);

    std::string getPassword ()const;
    void setPassword (const std::string &str);

    virtual void showInfo () const = 0 ;

    unsigned int getID ()const;
    void setID (const unsigned int ID_s);
};

class Reader : public Person
{
    friend class Data;
    friend class Admin;
private:
    std::vector<std::shared_ptr<Book>> BookBorrow;
    std::vector<std::string> TimeRecord;
    static std::map<unsigned int, std::shared_ptr<Reader>> ReaderList;
    bool tempOrder;//��¼�Ƿ�Ϊ��ʱ�������������Ҫд���ļ�����Ϊ����ʱ����trueΪ��ʱ����falseΪ��ʽ����
public:
    Reader () = default;
    Reader (const std::string &name_s, const std::string &password_s, unsigned int ID_s, int level);
    Reader (const Reader &Reader_s);
    Reader (Reader &&Reader_s);
    Reader (const std::string &name_s, const std::string &password_s);//�Զ�����ID��level
    ~Reader () {};

    int getCap()const;

    bool check_cap ();

    void borrowBook (unsigned int ID_s);
    void returnBook (unsigned int ID_s);

    void PrintBorrowedBooks ();
    std::vector<std::shared_ptr<Book>> &getBookBorrow();
    std::vector<std::string> &getTimeRecord();

    void showInfo () const override;

    void editInfo (const std::string &name_s, const std::string &password_s);

    static std::shared_ptr<Reader> Login(const unsigned int ID_s, const std::string &password_s);

    static std::map<unsigned int, std::shared_ptr<Reader>> &getReaderList();

    static std::vector<std::shared_ptr<Reader>> searchReaderByName (const std::string &name_s);
    static std::vector<std::shared_ptr<Reader>> searchReaderByID (const unsigned int ID_s);

    //ʹ������ô���static����������˵�����ǲ�����������ˣ���Ϊ�ӱ�����˵��static��ȫ�ֱ�����������Щ�����ı��ʾ���ȫ�ֺ����������ǳ�Ա����
};

class Admin : public Person
{
private:
    static std::map<unsigned int, std::shared_ptr<Admin>> AdminList;
    bool tempOrder;//��¼�Ƿ�Ϊ��ʱ�������������Ҫд���ļ�����Ϊ����ʱ����trueΪ��ʱ����falseΪ��ʽ����
public:
    Admin ();
    Admin (const std::string &name_s, const std::string &password_s, unsigned int ID_s, int level_s = 1);
    Admin (const Admin &Admin_s);
    Admin (Admin &&Admin_s);
    Admin (const std::string &name_s, const std::string &password_s);//�Զ�����ID��level
    ~Admin () {}

    std::shared_ptr<Reader> addReader (const std::string &name_s, const std::string &password_s, const unsigned int ID_s, const int level_s = 1);
    std::shared_ptr<Reader> addReader (const Reader &Reader_s);
    std::shared_ptr<Reader> addReader (Reader &&Reader_s);
    std::shared_ptr<Reader> addReader (const std::string &name_s, const std::string &password_s);//�Զ�����ID��level,Ĭ��levelΪ0��ʲôҲ������

    std::shared_ptr<Reader> editReader (std::shared_ptr<Reader> &res ,const std::string &name_s, const std::string &password_s, const int level_s);

    std::shared_ptr<Book> addBook (const std::string &name_s, const std::string &ISBN_s, const unsigned int ID_s, const bool status_s = 0);
    std::shared_ptr<Book> addBook (const Book &book_s);
    std::shared_ptr<Book> addBook (Book &&book_s);
    std::shared_ptr<Book> addBook (const std::string &name_s, const std::string &ISBN_s);//�Զ�����ID��Ĭ��Ϊ�ڿ�

    std::shared_ptr<Book> editBook (std::shared_ptr<Book> &res ,const std::string &name_s, const std::string &ISBN_s);

    std::shared_ptr<Admin>  addAdmin (const std::string &name_s, const std::string &password_s, const unsigned int ID_s, const int level_s = 1);
    std::shared_ptr<Admin>  addAdmin (const Admin &Admin_s);
    std::shared_ptr<Admin>  addAdmin (Admin &&Admin_s);
    std::shared_ptr<Admin>  addAdmin (const std::string &name_s, const std::string &password_s);//�Զ�����ID��level��Ĭ��levelΪ0

    std::shared_ptr<Admin> editAdmin (std::shared_ptr<Admin> &res ,const std::string &name_s, const std::string &password_s, const int level_s);

    enum {level0, level1, level2, level3, level4, level5};

    static std::map<unsigned int, std::shared_ptr<Admin>>& getAdminList ();

    void deleteReader (unsigned int ID_s);
    void deleteBook (unsigned int ID_s);
    void deleteAdmin (unsigned int ID_s);

    static std::shared_ptr<Admin> Login(unsigned int ID_s, const std::string &password_s);
    static std::vector<std::shared_ptr<Admin>> searchAdmin (const std::string &name_s);
    static std::vector<std::shared_ptr<Admin>> searchAdmin (const unsigned int ID_s);

    void showInfo () const override;

    void PrintReaderList ();
    void PrintBookList ();
    void PrintAdminList ();
    void PrintLogs ();
};


#endif