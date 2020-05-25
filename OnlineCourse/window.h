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
	const wchar_t* className_;      // 主窗口类名
	const wchar_t* windowTitle_;      // 主窗口类名
};


//class WindowButton
//{
//public:
//	WindowButton(string title, int x, int y, int w, int h, HWND windowId, HINSTANCE hInstance)
//	{
//		CreateWindow(
//			_T("BUTTON"),
//			_T("按钮1"), // 窗口标题
//			 // 样式，分别是，创建一个子窗口，创建的窗口可见，按钮(类型)
//			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
//			// 下面 4 个参数分别是窗口左上角的 x y 坐标以及窗口的 w h
//			0, 0, 400, 50,
//			windowId, // 按钮的父窗口，也就是放在哪个窗口里
//			(HMENU)0,           // 按钮的 ID，自行指定
//			hInstance,
//			NULL
//		);
//	}
//
//
//};

#endif //WINDOW_H