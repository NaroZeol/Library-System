#ifndef HANDLE_H_
#define HANDLE_H_
#include <string>
#include <fstream>
#include <iostream>
#include <chrono>
#include <ctime>
#include <functional>
//һЩ�����õ��Ĺ��ߺ���
//�������ռ��ܲ����ⲿ������
namespace handle{

std::string getCurrentTime();

void AddLogs (const std::string &str);

void PrintLogs ();

void ClearBuffer ();

void WrongInputCheck();

void WrongInputHandle();

void Pause();

template<typename T>
void InputData(std::istream &is, const char * str , T &data)//�޼�麯��
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

//��������Ƿ��������ڶ�����������ʾ��Ϣ��ֻҪ�������������һ�������Ǽ�麯��������ֵΪbool������Ϊ�����ֵ
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
//ģ�溯�������������룬���Է���ͷ�ļ���
}
#endif