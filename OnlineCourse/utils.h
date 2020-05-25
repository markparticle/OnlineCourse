/*
 * @Author       : mark
 * @Date         : 2020-05-20
 * @copyleft GPL 2.0
 */

#ifndef UTILS_H
#define UTILS_H

#include <iostream>

//log函数
template <class T>
void _print(T arg)
{
    std::cout << arg << " ";
}

template <class... Args>
void log(Args... args)
{
    //{(_print(args), 0)...}将会展开成((_print(arg1),0), (_print(arg2),0), (_print(arg3),0),  etc... 
    //由于是逗号表达式，在创建数组的过程中会先执行逗号表达式前面的部分_print(args)打印出参数
    //这个数组的目的纯粹是为了在数组构造的过程展开参数包
    int arr[] = { (_print(args), 0)... };
    std::cout << std::endl;
}

//测试函数
void ensure(bool condition, const std::string& message)
{
    if (condition)
    {
        log("测试成功");
    }
    else
    {
        log(message);
    }
}

#endif //UTILS_H