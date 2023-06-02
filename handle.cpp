#include "handle.h"

std::string handle::getCurrentTime()
 {
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::string timeStr = std::ctime(&time);
    timeStr.pop_back(); // 去除末尾的换行符
    return timeStr;
}

void handle::AddLogs (const std::string &str){
    std::fstream plog;
    plog.open("Data\\log.txt", std::ios::app);
    plog.clear();
    plog << str << std::endl;
    plog.close();
}

void handle::PrintLogs (){
    std::fstream plog;
    plog.open("Data\\log.txt", std::ios::in);
    plog.clear();
    std::string str;
    std::cout << "=====Logs=====\n";
    while (getline(plog, str))
    {
        std::cout << str << std::endl;
    }
    plog.close();
}


void handle::ClearBuffer ()
{
    while (std::cin.get() != '\n')
        continue;
}

void handle::WrongInputCheck()
{
    if (!std::cin.good())
        throw std::runtime_error("Wrong input!");
}

void handle::WrongInputHandle()
{
    std::cin.clear();
    handle::ClearBuffer();//清理缓冲区
    std::cerr << "Wrong input!\n";//输出错误信息，暂停的任务交给调用者
}

void handle::Pause()
{
    std::cout << "Press Enter to continue...";
    handle::ClearBuffer();
}

