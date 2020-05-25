/*
 * @Author       : mark
 * @Date         : 2020-05-20
 * @copyleft GPL 2.0
 */
#ifndef WINDOW_H
#define WINDOW_H

#include <Windows.h>
#include <tchar.h>
#include <fstream>
#include <sstream>
#include <atlstr.h>

enum WindowTag
{
	LoginButtonTag = 1, 
	UserNameInputTag,
	UserPasswordInputTag,
};

class MainWindow
{
public:

	//INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	MainWindow(HINSTANCE hInstance);
	void MyReisterClass();
	void InitInstance();
	int runLoop();

	void buttonAction(int buttonId);
	//std::vector<WindowButton> buttonList_;
private:
	HWND windowId_;
	HINSTANCE instanceId_;
	const wchar_t* className_;      // ����������
	const wchar_t* windowTitle_;      // ����������
};


//class WindowButton
//{
//public:
//	WindowButton(string title, int x, int y, int w, int h, HWND windowId, HINSTANCE hInstance)
//	{
//		CreateWindow(
//			_T("BUTTON"),
//			_T("��ť1"), // ���ڱ���
//			 // ��ʽ���ֱ��ǣ�����һ���Ӵ��ڣ������Ĵ��ڿɼ�����ť(����)
//			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
//			// ���� 4 �������ֱ��Ǵ������Ͻǵ� x y �����Լ����ڵ� w h
//			0, 0, 400, 50,
//			windowId, // ��ť�ĸ����ڣ�Ҳ���Ƿ����ĸ�������
//			(HMENU)0,           // ��ť�� ID������ָ��
//			hInstance,
//			NULL
//		);
//	}
//
//
//};

#endif //WINDOW_H