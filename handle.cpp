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


void handle::ClearBuffer (std::istream &is)
{
    while (is.get() != '\n' && is.good())
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

int handle::getAbsoluteTime (const std::string &str)
{
    
    std::string date1_str;
    if (str.empty())
        date1_str = getCurrentTime();
    else
        date1_str = str;
    std::string base_date_str = "Sat Jan 01 00:00:00 2022";

    std::tm date1_tm = {};
    std::tm base_date_tm = {};

    std::istringstream ss1(date1_str);
    std::istringstream ss2(base_date_str);

    ss1 >> std::get_time(&date1_tm, "%a %b %d %H:%M:%S %Y");
    ss2 >> std::get_time(&base_date_tm, "%a %b %d %H:%M:%S %Y");

    std::time_t date1_time = mktime(&date1_tm);
    std::time_t base_date_time = mktime(&base_date_tm);

    double seconds_diff = difftime(date1_time, base_date_time);
    int days_diff = seconds_diff / (60 * 60 * 24);

    return days_diff;
}

std::string handle::hash_str_to_str (const std::string &str)
{
    std::hash<std::string> hash_str;
    std::string str_hash = std::to_string(hash_str(str));
    return str_hash;
}