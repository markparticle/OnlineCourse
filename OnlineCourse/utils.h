/*
 * @Author       : mark
 * @Date         : 2020-05-20
 * @copyleft GPL 2.0
 */

#ifndef UTILS_H
#define UTILS_H

#include <iostream>

//log����
template <class T>
void _print(T arg)
{
    std::cout << arg << " ";
}

template <class... Args>
void log(Args... args)
{
    //{(_print(args), 0)...}����չ����((_print(arg1),0), (_print(arg2),0), (_print(arg3),0),  etc... 
    //�����Ƕ��ű��ʽ���ڴ�������Ĺ����л���ִ�ж��ű��ʽǰ��Ĳ���_print(args)��ӡ������
    //��������Ŀ�Ĵ�����Ϊ�������鹹��Ĺ���չ��������
    int arr[] = { (_print(args), 0)... };
    std::cout << std::endl;
}

//���Ժ���
void ensure(bool condition, const std::string& message)
{
    if (condition)
    {
        log("���Գɹ�");
    }
    else
    {
        log(message);
    }
}

#endif //UTILS_H