#include "Person.h"
#include "handle.h"

Person::Person(const std::string &name_s, const std::string &password_s, unsigned int ID_s, int level_s):
    name(name_s), password(handle::hash_str_to_str(password_s)), ID(ID_s), level(level_s){}

int Person::getLevel()const{return level;}

void Person::setLevel(const int level_s){level = level_s;}

std::string Person::getPassword ()const{return password;}

void Person::setPassword (const std::string &str){password = handle::hash_str_to_str(str);}

unsigned int Person::getID ()const{return ID;}

void Person::setID (const unsigned int ID_s){ID = ID_s;}

void Person::setName (const std::string &str){name = str;}

std::string Person::getName() const{return name;}

std::map<unsigned int, std::shared_ptr<Reader>> &Reader::getReaderList(){return Reader::ReaderList;}

Reader::Reader(const std::string &name_s, const std::string &password_s, unsigned int ID_s, int level):
    Person(name_s, password_s, ID_s, level), tempOrder(true) {}

Reader::Reader (const Reader &Reader_s):
    Person(Reader_s.name, Reader_s.password, Reader_s.ID, Reader_s.level), BookBorrow(Reader_s.BookBorrow), tempOrder(Reader_s.tempOrder){}

Reader::Reader (Reader &&Reader_s):
    Person(Reader_s.name, Reader_s.password, Reader_s.ID, Reader_s.level), BookBorrow(std::move(Reader_s.BookBorrow)), tempOrder(Reader_s.tempOrder){}

Reader::Reader (const std::string &name_s, const std::string &password_s):
    Person(name_s, password_s, 0, 0), tempOrder(true)
{
    unsigned int ID_s = 0;
    while (Reader::getReaderList().find(ID_s) != Reader::getReaderList().end())
        ++ID_s;
    ID = ID_s;
    level = 0;
}

int Reader::getCap ()const {return level + 3;}

bool Reader::check_cap (){return BookBorrow.size() < level + 3;}

std::shared_ptr<Reader> Reader::Login (const unsigned int ID_s, const std::string &password_s)
{
    auto &ReaderList = Reader::getReaderList();
    std::string ps_str = handle::hash_str_to_str(password_s);

    if (ReaderList.find(ID_s) != ReaderList.end()){
        if (ReaderList[ID_s]->getPassword() == ps_str){
            return ReaderList[ID_s];
        }
        else
            std::cout << "Password Error!\n";
    }
    
    return nullptr;
}

void Reader::borrowBook (unsigned int ID_s)
{
    auto booklis = Book::getIDBookList();
    auto it =  booklis.find(ID_s);
    if (it == booklis.end()){
        std::cout << "Book Not Found!\n";
        return;
    }

    if (it->second->getStatus() == 1){
        std::cout << "Book has been borrowed!\n";
        return;
    }

    if (check_cap()){
        BookBorrow.push_back(it->second);
        TimeRecord.push_back(std::move(handle::getCurrentTime()));
        it->second->setStatus(1);
        std::cout << "Borrow Success!\n";
        handle::AddLogs("Reader " + getName() + "(ID: " + std::to_string(getID()) + ")" + " borrow book " + it->second->getName() +  "(ID: " + std::to_string(it->second->getID()) + ")" + " at " + handle::getCurrentTime());
    }
    else
        std::cout << "You have borrowed too many books!\n";
}

void Reader::returnBook (unsigned int ID_s)
{
    if (ReaderList.find(this->getID()) == ReaderList.end()){
        std::cout << "This reader doesn't exist!\n";
        return;
    }
    auto booklis = Book::getIDBookList();
    auto it =  booklis.find(ID_s);
    if (it == booklis.end()){
        std::cout << "Book Not Found!\n";
        return;
    }

    if (it->second->getStatus() == 0){
        std::cout << "Book has been returned!\n";
        return;
    }

    for (auto i = BookBorrow.begin(); i != BookBorrow.end(); ++i){
        if ((*i)->getID() == ID_s){
            BookBorrow.erase(i);
            it->second->setStatus(0);
            std::cout << "Return Success!\n";

            if (handle::getAbsoluteTime() - handle::getAbsoluteTime(TimeRecord[i - BookBorrow.begin()]) > 14)
                std::cout << "You have overdue!\n";

            TimeRecord.erase(TimeRecord.begin() + (i - BookBorrow.begin()));

            handle::AddLogs("Reader " + getName() +  "(ID: " + std::to_string(getID()) + ")" + " return book " + it->second->getName() +  "(ID: " + std::to_string(it->second->getID()) + ")" + " at " + handle::getCurrentTime());
            return;
        }
    }
    std::cout << "You don't borrow this book!\n";
    handle::AddLogs ("Reader " + getName() +  "(ID: " + std::to_string(getID()) + ")" + " return book " + it->second->getName() +  "(ID: " + std::to_string(it->second->getID()) + ")" + " at " + handle::getCurrentTime());
}

void Reader::showInfo() const 
{
    std::cout << "Name: " << name << std::endl;
    std::cout << "ID: " << ID << std::endl;
    std::cout << "Level: " << level << std::endl;
    std::cout << "Capacity: " << level + 3 << std::endl;
}

void Reader::editInfo (const std::string &name_s, const std::string &password_s)
{
    name = name_s;
    password = handle::hash_str_to_str(password_s);
    if (tempOrder == false)
        handle::AddLogs("Reader ID " + std::to_string(ID) + " edited his/her information.");
}

void Reader::PrintBorrowedBooks ()
{
    std::cout << "=====Borrowed Books=====\n";
    if (BookBorrow.empty()){
        std::cout << "None\n";
        return;
    }
    for (auto i = BookBorrow.begin(); i != BookBorrow.end(); ++i){
        std::cout << "Book Name: " << (*i)->getName() << std::endl;
        std::cout << "Book ID: " << (*i)->getID() << std::endl;
        std::cout << "Book ISBN: " << (*i)->getISBN() << std::endl;
        std::cout << "Borrow Time: " << TimeRecord[i - BookBorrow.begin()] << std::endl;
        std::cout << std::endl;
    }
    return;
}

std::vector<std::shared_ptr<Book>> &Reader::getBookBorrow() {return BookBorrow;}

std::vector<std::string> &Reader::getTimeRecord() {return TimeRecord;}

std::vector<std::shared_ptr<Reader>> Reader::searchReaderByName (const std::string &name_s)
{
    auto &ReaderList = Reader::getReaderList();
    std::vector<std::shared_ptr<Reader>> temp;
    for (auto &i : ReaderList)
        if (i.second->getName() == name_s)
            temp.push_back(i.second);
    return temp;
}

std::vector<std::shared_ptr<Reader>> Reader::searchReaderByID (const unsigned int ID_s)
{
    auto &ReaderList = Reader::getReaderList();
    std::vector<std::shared_ptr<Reader>> temp;
    if (ReaderList.find(ID_s) != ReaderList.end())
        temp.push_back(ReaderList[ID_s]);
    return temp;
}

std::map<unsigned int, std::shared_ptr<Reader>> Reader::ReaderList;

std::map<unsigned int, std::shared_ptr<Admin>> Admin::AdminList;

Admin::Admin():
    Person("Admin", "Admin", 0, 5), tempOrder(true){}

Admin::Admin(const std::string &name_s, const std::string &password_s, unsigned int ID_s, int level_s):
    Person(name_s, password_s, ID_s, level_s), tempOrder(true){}

Admin::Admin (const Admin &Admin_s) :
    Person(Admin_s.name, Admin_s.password, Admin_s.ID, Admin_s.level), tempOrder(Admin_s.tempOrder){}

Admin::Admin (Admin &&Admin_s) :
    Person(Admin_s.name, Admin_s.password, Admin_s.ID, Admin_s.level), tempOrder(Admin_s.tempOrder){}

Admin::Admin (const std::string &name_s, const std::string &password_s):
    Person(name_s, password_s, 0, 0), tempOrder(true)
{
    unsigned int ID_s = 1;
    while (Admin::getAdminList().find(ID_s) != Admin::getAdminList().end())
        ++ID_s;
    ID = ID_s;
    level = 0;
}

std::map<unsigned int, std::shared_ptr<Admin>> &Admin::getAdminList (){return AdminList;}

std::shared_ptr<Admin> Admin::Login (const unsigned int ID_s, const std::string &password_s)
{
    auto &AdminList = Admin::getAdminList();
    std::string ps_str = handle::hash_str_to_str(password_s);
    if (AdminList.find(ID_s) != AdminList.end()){
        if (AdminList[ID_s]->getPassword() == ps_str){
            return AdminList[ID_s];
        }
        else
            std::cout << "Password Error!\n";
    }
    
    return nullptr;
}

std::shared_ptr<Admin> Admin::addAdmin (const std::string &name_s, const std::string &password_s, const unsigned int ID_s, const int level_s)
{
    if (level < 4){
        std::cout << this->getName() + " don't have the permission!\n";
        return nullptr;
    }
    if (Admin::getAdminList().find(ID_s) != Admin::getAdminList().end()){
        std::cout << "ID has been used!\n";
        return nullptr;
    }
    auto ptr = std::make_shared<Admin>(name_s, password_s, ID_s, level_s);
    Admin::getAdminList().insert(std::make_pair(ID_s, ptr));

    if (tempOrder == false)
        handle::AddLogs("Admin " + this->getName() +  "(ID: " + std::to_string(this->getID()) + ")" + " Add Admin " + name_s +  "(ID: " + std::to_string(ptr->getID()) + ")" + " at " + handle::getCurrentTime());
    ptr->tempOrder = false;
    return ptr;
}

std::shared_ptr<Admin> Admin::addAdmin (const Admin &Admin_s)
{
    if (level < 4){
        std::cout << this->getName() + " don't have the permission!\n";
        return nullptr;
    }
    if (Admin::getAdminList().find(Admin_s.getID()) != Admin::getAdminList().end()){
        std::cout << "ID has been used!\n";
        return nullptr;
    }
    auto ptr = std::make_shared<Admin>(Admin_s);
    Admin::getAdminList().insert(std::make_pair(Admin_s.getID(), ptr));
    if (tempOrder == false)
        handle::AddLogs("Admin " + this->getName() +  "(ID: " + std::to_string(this->getID()) + ")" + " Add Admin " + ptr->getName() +  "(ID: " + std::to_string(ptr->getID()) + ")" + " at " + handle::getCurrentTime());
    ptr->tempOrder = false;
    return ptr;
}

std::shared_ptr<Admin> Admin::addAdmin (Admin &&Admin_s)
{
    if (level < 4){
        std::cout << this->getName() + " don't have the permission!\n";
        return nullptr;
    }
    if (Admin::getAdminList().find(Admin_s.getID()) != Admin::getAdminList().end()){
        std::cout << "ID has been used!\n";
        return nullptr;
    }
    auto ptr = std::make_shared<Admin>(std::move(Admin_s));
    Admin::getAdminList().insert(std::make_pair(Admin_s.getID(), ptr));
    if (tempOrder == false)
        handle::AddLogs("Admin " + this->getName() +  "(ID: " + std::to_string(this->getID()) + ")" + " Add Admin " + ptr->getName() +  "(ID: " + std::to_string(ptr->getID()) + ")" + " at " + handle::getCurrentTime());
    ptr->tempOrder = false;
    return ptr;
}

std::shared_ptr<Admin>  Admin::addAdmin (const std::string &name_s, const std::string &password_s)
{
    if (level < 4){
        std::cout << this->getName() + " don't have the permission!\n";
        return nullptr;
    }

    Admin temp(name_s, password_s);
    if (Admin::getAdminList().find(temp.getID()) != Admin::getAdminList().end()){
        std::cout << "ID has been used!\n";
        return nullptr;
    }
    auto ptr = addAdmin(temp);
    ptr->tempOrder = false;
    return ptr;
}

std::shared_ptr<Reader> Admin::addReader (const std::string &name_s, const std::string &password_s, const unsigned int ID_s, const int level_s)
{
    if (level < 3){
        std::cout << this->getName() + " don't have the permission!\n";
        return nullptr;
    }

    if (Reader::getReaderList().find(ID_s) != Reader::getReaderList().end()){
        std::cout << "ID has been used!\n";
        return nullptr;
    }
    auto ptr = std::make_shared<Reader>(name_s, password_s, ID_s, level_s);
    Reader::getReaderList().insert(std::make_pair(ID_s, ptr));
    if (tempOrder == false)
        handle::AddLogs("Admin " + this->getName() + "(ID: " + std::to_string(this->getID()) + ")" + " Add Reader " + ptr->getName() +  "(ID: " + std::to_string(ptr->getID()) + ")" + " at " + handle::getCurrentTime());
    ptr->tempOrder = false;
    return ptr;
}

std::shared_ptr<Reader> Admin::addReader (const Reader &Reader_s)
{
    if (level < 3){
        std::cout << this->getName() + " don't have the permission!\n";
        return nullptr;
    }
    if (Reader::getReaderList().find(Reader_s.getID()) != Reader::getReaderList().end()){
        std::cout << "ID has been used!\n";
        return nullptr;
    }
    auto ptr = std::make_shared<Reader>(Reader_s);
    Reader::getReaderList().insert(std::make_pair(Reader_s.getID(), ptr));
    if (tempOrder == false)
        handle::AddLogs("Admin " + this->getName() + "(ID: " + std::to_string(this->getID()) + ")" + " Add Reader " + ptr->getName() +  "(ID: " + std::to_string(ptr->getID()) + ")" + " at " + handle::getCurrentTime());
    ptr->tempOrder = false;
    return ptr;
}

std::shared_ptr<Reader> Admin::addReader (Reader &&Reader_s)
{
    if (level < 3){
        std::cout << this->getName() + " don't have the permission!\n";
        return nullptr;
    }
    if (Reader::getReaderList().find(Reader_s.getID()) != Reader::getReaderList().end()){
        std::cout << "ID has been used!\n";
        return nullptr;
    }
    auto ptr = std::make_shared<Reader>(std::move(Reader_s));
    Reader::getReaderList().insert(std::make_pair(Reader_s.getID(), ptr));
    if (tempOrder == false)
        handle::AddLogs("Admin " + this->getName() + "(ID: " + std::to_string(this->getID()) + ")" + " Add Reader " + ptr->getName() +  "(ID: " + std::to_string(ptr->getID()) + ")" + " at " + handle::getCurrentTime());
    ptr->tempOrder = false;
    return ptr;
}

std::shared_ptr<Reader> Admin::addReader (const std::string &name_s, const std::string &password_s){
    if (level < 3){
        std::cout << this->getName() + " don't have the permission!\n";
        return nullptr;
    }
    Reader temp(name_s, password_s);
    if (Reader::getReaderList().find(temp.getID()) != Reader::getReaderList().end()){
        std::cout << "ID has been used!\n";
        return nullptr;
    }
    auto ptr = addReader(temp);
    if (tempOrder == false)
        handle::AddLogs("Admin " + this->getName() + "(ID: " + std::to_string(this->getID()) + ")" + " Add Reader " + ptr->getName() +  "(ID: " + std::to_string(ptr->getID()) + ")" + " at " + handle::getCurrentTime());
    ptr->tempOrder = false;
    return ptr;
}

std::shared_ptr<Reader> Admin::editReader (std::shared_ptr<Reader> &res ,const std::string &name_s, const std::string &password_s, const int level_s)
{
    if (level < 3){
        std::cout << this->getName() + " don't have the permission!\n";
        return nullptr;
    }
    if (Reader::getReaderList().find(res->getID()) == Reader::getReaderList().end()){
        std::cout << "Reader Not Found!\n";
        return nullptr;
    }
    res->setName(name_s);
    res->setPassword(password_s);
    res->setLevel(level_s);
    if (tempOrder == false)
        handle::AddLogs("Admin " + this->getName() + "(ID: " + std::to_string(this->getID()) + ")" +  " Edit Reader " + res->getName() +  "(ID: " + std::to_string(res->getID()) + ")" + " at " + handle::getCurrentTime());
    return res;
}

std::shared_ptr<Book> Admin::addBook (const std::string &name_s, const std::string &ISBN_s, const unsigned int ID_s, const bool status_s)
{
    if (level < 3){
        std::cout << this->getName() + " don't have the permission!\n";
        return nullptr;
    }    
    if (Book::getIDBookList().find(ID_s) != Book::getIDBookList().end()){
        std::cout << "ID has been used!\n";
        return nullptr;
    }
    auto ptr = std::make_shared<Book>(name_s, ISBN_s, ID_s, status_s);
    Book::getISBNBookList().insert(std::make_pair(ISBN_s, ptr));
    Book::getIDBookList().insert(std::make_pair(ID_s, ptr));
    if (tempOrder == false)
        handle::AddLogs("Admin " + this->getName() +  "(ID: " + std::to_string(this->getID()) + ")" + " Add Book " + ptr->getName() + "(ID: " + std::to_string(ptr->getID()) + ")" +  " at " + handle::getCurrentTime());
    return ptr;
}

std::shared_ptr<Book> Admin::addBook(const Book &book_s)
{
    if (level < 3){
        std::cout << this->getName() + " don't have the permission!\n";
        return nullptr;
    }    
    if (Book::getIDBookList().find(book_s.getID()) != Book::getIDBookList().end()){
        std::cout << "ID has been used!\n";
        return nullptr;
    }
    auto ptr = std::make_shared<Book>(book_s);
    Book::getISBNBookList().insert(std::make_pair(book_s.getISBN(), ptr));
    Book::getIDBookList().insert(std::make_pair(book_s.getID(), ptr));
    if (tempOrder == false)
        handle::AddLogs("Admin " + this->getName() +  "(ID: " + std::to_string(this->getID()) + ")" + " Add Book " + ptr->getName() + "(ID: " + std::to_string(ptr->getID()) + ")" +  " at " + handle::getCurrentTime());
    return ptr;
}

std::shared_ptr<Book> Admin::addBook (const std::string &name_s, const std::string &ISBN_s)
{
    if (level < 3){
        std::cout << this->getName() + " don't have the permission!\n";
        return nullptr;
    }    
    Book temp(name_s, ISBN_s);
    if (Book::getIDBookList().find(temp.getID()) != Book::getIDBookList().end()){
        std::cout << "ID has been used!\n";
        return nullptr;
    }
    auto ptr = addBook(temp);//用其他函数作为代理
    return ptr;
}

std::shared_ptr<Book> Admin::addBook(Book &&book_s)
{
    if (level < 3){
        std::cout << this->getName() + " don't have the permission!\n";
        return nullptr;
    }    
    if (Book::getIDBookList().find(book_s.getID()) != Book::getIDBookList().end()){
        std::cout << "ID has been used!\n";
        return nullptr;
    }
    auto ptr = std::make_shared<Book>(book_s);
    Book::getISBNBookList().insert(std::make_pair(book_s.getISBN(), ptr));
    Book::getIDBookList().insert(std::make_pair(book_s.getID(), ptr));
    if (tempOrder == false)
        handle::AddLogs("Admin " + this->getName() +  "(ID: " + std::to_string(this->getID()) + ")" + " Add Book " + ptr->getName() + "(ID: " + std::to_string(ptr->getID()) + ")" +  " at " + handle::getCurrentTime());
    return ptr;
}

std::shared_ptr<Admin> Admin::editAdmin (std::shared_ptr<Admin> &res ,const std::string &name_s, const std::string &password_s, const int level_s)
{
    if (level < 4){
        std::cout << this->getName() + " don't have the permission!\n";
        return nullptr;
    }
    if (Admin::getAdminList().find(res->getID()) == Admin::getAdminList().end()){
        std::cout << "Admin Not Found!\n";
        return nullptr;
    }
    res->setName(name_s);
    res->setPassword(password_s);
    res->setLevel(level_s);
    if (tempOrder == false)
        handle::AddLogs("Admin " + this->getName() +  "(ID: " + std::to_string(this->getID()) + ")" + " Edit Admin " + res->getName() + "(ID: " + std::to_string(res->getID()) + ")" +  " at " + handle::getCurrentTime());
    return res;
}


std::shared_ptr<Book> Admin::editBook (std::shared_ptr<Book> &res ,const std::string &name_s, const std::string &ISBN_s)
{
    if (level < 3){
        std::cout << this->getName() + " don't have the permission!\n";
        return nullptr;
    }
    if (Book::getIDBookList().find(res->getID()) == Book::getIDBookList().end()){
        std::cout << "Book Not Found!\n";
        return nullptr;
    }
    res->setISBN(ISBN_s);
    res->setName(name_s);

    if (tempOrder == false)
        handle::AddLogs("Admin " + this->getName() +  "(ID: " + std::to_string(this->getID()) + ")" + " Edit Book " + res->getName() + "(ID: " + std::to_string(res->getID()) + ")" +  " at " + handle::getCurrentTime());
    return res;
}

void Admin::deleteReader (unsigned int ID_s)
{
    if (level < 3){
        std::cout << this->getName() + " don't have the permission!\n";
        return;
    }    
    auto &ReaderList = Reader::getReaderList();
    if (ReaderList.find(ID_s) != ReaderList.end()){
        if (tempOrder == false)
            handle::AddLogs("Admin" + this->getName() + "(ID: " + std::to_string(this->getID()) + ")" +  "Delete Reader " + ReaderList[ID_s]->getName() + "(ID: " + std::to_string(ReaderList[ID_s]->getID()) + ")" +  " at " + handle::getCurrentTime());
        ReaderList.erase(ID_s);
        return;
    }
    else
        std::cout << "Reader Not Found!\n";
}

void Admin::deleteBook (unsigned int ID_s)
{
    if (level < 3){
        std::cout << this->getName() + " don't have the permission!\n";
        return;
    }    
    auto it = Book::getIDBookList().find(ID_s);
    if (it == Book::getIDBookList().end()){
        std::cout << "Book Not Found!\n";
        return;
    }
    if (tempOrder == false)
        handle::AddLogs("Admin " + this->getName() +  "(ID: " + std::to_string(this->getID()) + ")" + " Delete Book " + it->second->getName() + "(ID: " + std::to_string(it->second->getID()) + ")" +  " at " + handle::getCurrentTime());
    Book::getISBNBookList().erase(it->second->getISBN());
    Book::getIDBookList().erase(it);

}

void Admin::deleteAdmin (unsigned int ID_s)
{
    if (level < 4){
        std::cout << this->getName() + " don't have the permission!\n";
        return;
    }
    auto &AdminList = Admin::getAdminList();
    if (AdminList.find(ID_s) != AdminList.end()){
        if (tempOrder == false)
            handle::AddLogs("Admin " + this->getName() +  "(ID: " + std::to_string(this->getID()) + ")" + " Delete Admin " + AdminList[ID_s]->getName() + "(ID: " + std::to_string(AdminList[ID_s]->getID()) + ")" +  " at " + handle::getCurrentTime());
        AdminList.erase(ID_s);
        return;
    }
    else
        std::cout << "Admin Not Found!\n";
}

std::vector<std::shared_ptr<Admin>> Admin::searchAdmin (const std::string &name_s)
{
    auto &AdminList = Admin::getAdminList();
    std::vector<std::shared_ptr<Admin>> temp;
    for (auto &i : AdminList)
        if (i.second->getName() == name_s)
            temp.push_back(i.second);
    return temp;
}

std::vector<std::shared_ptr<Admin>> Admin::searchAdmin (const unsigned int ID_s)
{
    auto &AdminList = Admin::getAdminList();
    std::vector<std::shared_ptr<Admin>> temp;
    if (AdminList.find(ID_s) != AdminList.end())
        temp.push_back(AdminList[ID_s]);
    return temp;
}

void Admin::PrintReaderList ()
{
    if (level < 2){
        std::cout << this->getName() + " don't have the permission!\n";
        return;
    }
    auto &ReaderList = Reader::getReaderList();
    std::cout << "=====Reader List=====\n";
    for (auto &i : ReaderList){
        std::cout << "Name: " << i.second->getName() << std::endl;
        std::cout << "ID: " << i.second->getID() << std::endl;
        std::cout << "Level: " << i.second->getLevel() << std::endl;
        std::cout << "Capacity: " << i.second->getCap() << std::endl;
        std::cout << "Borrowed Books: ";
        if (i.second->getBookBorrow().empty()){
            std::cout << "None\n";
        }
        else
            for (auto &j : i.second->getBookBorrow())
                std::cout << j->getName() << " ";
        std::cout << "\n" << std::endl;
    }
}

void Admin::PrintBookList(){
    if (level < 2){
        std::cout << this->getName() + " don't have the permission!\n";
        return;
    }
    std::cout << "=====Book List=====\n";
    auto BookList = Book::getIDBookList();
    for (auto &i : BookList){
        std::cout << "Name: " << i.second->getName() << std::endl;
        std::cout << "ID: " << i.second->getID() << std::endl;
        std::cout << "ISBN: " << i.second->getISBN() << std::endl;
        std::cout << "Status: " << (i.second->getStatus() ? "Borrowed" : "In Library") << std::endl;
        std::cout << std::endl;
    }
}

void Admin::PrintAdminList()
{
    if (level < 4){
        std::cout << this->getName() + " don't have the permission!\n";
        return;
    }
    std::cout << "=====Admin List=====\n";
    auto AdminList = Admin::getAdminList();
    for (auto &i : AdminList){
        std::cout << "Name: " << i.second->getName() << std::endl;
        std::cout << "ID: " << i.second->getID() << std::endl;
        std::cout << "Level: " << i.second->getLevel() << std::endl;
        std::cout << std::endl;
    }
}

void Admin::PrintLogs()
{
    if (level < 1){
        std::cout << this->getName() + " don't have the permission!\n";
        return;
    }
    handle::PrintLogs();
}

void Admin::showInfo() const 
{
    std::cout << "Name: " << name << std::endl;
    std::cout << "ID: " << ID << std::endl;
    std::cout << "Level: " << level << std::endl;
}