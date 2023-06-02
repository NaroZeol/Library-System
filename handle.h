#ifndef HANDLE_H_
#define HANDLE_H_
#include <string>
#include <fstream>
#include <iostream>
#include <chrono>
#include <ctime>
#include <functional>
//一些可能用到的工具函数
//用命名空间总不算外部函数吧
namespace handle{

std::string getCurrentTime();

void AddLogs (const std::string &str);

void PrintLogs ();

void ClearBuffer ();

void WrongInputCheck();

void WrongInputHandle();

void Pause();

template<typename T>
void InputData(std::istream &is, const char * str , T &data)//无检查函数
{
    while(true){
        is >> data;
        if ( is.fail() ){
            is.clear();
            handle::ClearBuffer();
            std::cerr << "Wrong input!\n";
            handle::Pause();
            std::cout << "Please enter the "+ std::string(str) +" again: ";
            continue;
        }   

        break;
    }

    handle::ClearBuffer();
}

//检查输入是否正常，第二个参数是提示信息（只要变量名），最后一个参数是检查函数，返回值为bool，参数为输入的值
template<typename T, typename Func>
void InputData(std::istream &is, const char * str , T &data, Func check)
{
    while(true){
        is >> data;
        if ( is.fail() ){
            is.clear();
            handle::ClearBuffer();
            std::cerr << "Wrong input!\n";
            handle::Pause();
            std::cout << "Please enter the "+ std::string(str) +" again: ";
            continue;
        }   

        if ( !check(data) ){
            std::cerr << "Wrong input!\n";
            handle::Pause();
            std::cout << "Please enter the "+ std::string(str) +" again: ";
            continue;
        }

        break;
    }

    handle::ClearBuffer();
}
//模版函数不建议分离编译，所以放在头文件里
}
#endif