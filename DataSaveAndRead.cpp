#include "DataSaveAndRead.h"
//Reader
//Admin
//Book

void Data::Save(){
    std::ofstream os; 
//ReaderList
    os.open("Data\\ReaderInfo", std::ios::out|std::ios::trunc);
    auto &ReaderList = Reader::getReaderList();
//INFO_Order:name, password, ID, level, BookBorrow(Save as BookID) and TimeRecord 
    for (auto &i : ReaderList){
        os << i.second->getName() << "|";
        os << i.second->getPassword() << "|";
        os << i.second->getID() << "|";
        os << i.second->getLevel() << "|";
        for (unsigned int j = 0; j < i.second->BookBorrow.size(); j++){
            os << i.second->BookBorrow[j]->getID() << "|";
            os << i.second->TimeRecord[j] << "|";
        }
        os << std::endl;
    }
    os.close();

//AdminList
    os.open("Data\\AdminInfo", std::ios::out|std::ios::trunc);
    auto &AdminList = Admin::getAdminList();
//INFO_Order:name, password, ID, level
    for (auto &i : AdminList){
        os << i.second->getName() << "|";
        os << i.second->getPassword() << "|";
        os << i.second->getID() << "|";
        os << i.second->getLevel() << "|";
        os << std::endl;
    }
    os.close();

//BookList
    os.open("Data\\BookInfo", std::ios::out|std::ios::trunc);
    auto &BookList = Book::getIDBookList();//This is a map
//INFO_Order: name, ISBN, ID, status
    for (auto &i : BookList){
        os << i.second->getName() << "|";
        os << i.second->getISBN() << "|";
        os << i.second->getID() << "|";
        os << i.second->getStatus() << "|";
        os << std::endl;
    }
    os.close();
}




void Data::Read(){
    std::ifstream is;
    Admin tempAdmin("temp", "111", 50, 5);
    std::string line;

    std::string name_s, password_s;
    unsigned int ID_s;
    int level_s, cap;
//BookList
    is.open ("Data\\BookInfo", std::ios::in);
//INFO_Order: name, ISBN, ID, status
    while (std::getline(is, line))
    {
        bool status_s;
        std::string ISBN_s;
        std::stringstream ssin(line);

        enum {
            name, ISBN, ID, status
        };
        int choice = name;
        while (choice <= status)
        {
            std::stringstream tempsteam;
            while (ssin.peek() != '|' && ssin.peek() != EOF){
                tempsteam << (char)ssin.get();
            }
            ssin.get();
            switch (choice++)
            {
            case name:
                std::getline(tempsteam, name_s);
                break;
            case ISBN:
                std::getline(tempsteam, ISBN_s);
                break;;
            case ID:
                tempsteam >> ID_s;
                break;
            case status:
                tempsteam >> status_s;
                break;
            default:
                break;
            }
        }
        
        tempAdmin.addBook(name_s, ISBN_s, ID_s, status_s);
    }
    is.close();

//ReaderList
    is.open ("Data\\ReaderInfo", std::ios::in);
//INFO_Order:name, password, ID, level, cap, BookBorrow and TimeRecord(save as BookID)
    while (std::getline(is, line)){
        std::stringstream ssin(line);
        enum {
            name, password, ID, level
        };
        int choice = name;
        while (choice <= level)
        {
            std::stringstream tempsteam;
            while (ssin.peek() != '|' && ssin.peek() != EOF)
                tempsteam << (char)ssin.get();
            ssin.get();
            switch (choice++)
            {
            case name:
                std::getline(tempsteam, name_s);
                break;
            case password:
                std::getline(tempsteam, password_s);
                break;;
            case ID:
                tempsteam >> ID_s;
                break;
            case level:
                tempsteam >> level_s;
                break;
            default:
                break;
            }
        }

        auto create = tempAdmin.addReader(name_s, password_s, ID_s, level_s);
        create->password = password_s;
        auto &BookList = Book::getIDBookList();
        unsigned int temp = 0;
        std::string timerecord;

        bool flag = true;//为真时读取的是BookID，为假时读取的是TimeRecord
        
        while (ssin.good() && ssin.peek() != EOF)
        {
            std::stringstream tempsteam;
            while (ssin.peek() != '|' && ssin.peek() != EOF)
                tempsteam << (char)ssin.get();
            ssin.get();
            if (flag)
            {
                tempsteam >> temp;
                if (BookList.count(temp))
                    create->BookBorrow.push_back(BookList[temp]);
                flag = false;
            }
            else
            {
                std::getline(tempsteam, timerecord);
                create->TimeRecord.push_back(timerecord);
                flag = true;
            }
        }
    }

    is.clear();
    is.close();

//AdminList
    is.open ("Data\\AdminInfo", std::ios::in);
//INFO_Order:name, password, ID, level
    while (std::getline(is, line)){
        std::stringstream ssin(line);
        enum {
            name, password, ID, level
        };
        int choice = name;
        while (choice <= level)
        {
            std::stringstream tempsteam;
            while (ssin.peek() != '|' && ssin.peek() != EOF)
                tempsteam << (char)ssin.get();
            ssin.get();
            switch (choice++)
            {
            case name:
                std::getline(tempsteam, name_s);
                break;
            case password:
                std::getline(tempsteam, password_s);
                break;;
            case ID:
                tempsteam >> ID_s;
                break;
            case level:
                tempsteam >> level_s;
                break;
            default:
                break;
            }
        }
        tempAdmin.addAdmin(name_s, password_s, ID_s, level_s)->password = password_s;
    }
    is.clear();
    is.close();
}
