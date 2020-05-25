/*
 * @Author       : mark
 * @Date         : 2020-05-24
 * @copyleft GPL 2.0
 */
#include <iostream>
#include "window.h"

using namespace std;


MainWindow::MainWindow(HINSTANCE hInstance): instanceId_(hInstance){
	className_ = _T("WindowClass");
	windowTitle_ = _T("雷课堂");
	InitInstance();
	runLoop();
}

void MainWindow::InitInstance() {
	// 创建主窗口
	MyReisterClass();
	windowId_ = CreateWindow(
		className_,                  // 这个要和上面的 window 的 lpszClassName 属性一样
		windowTitle_, // 窗口标题
		WS_OVERLAPPEDWINDOW,        // 窗口样式
		// 下面 4 个参数分别是窗口左上角的 x y 坐标以及窗口的 w h
		100, 100, 800, 600,
		// 下面 4 个现在应当做规定
		NULL,
		NULL,
		instanceId_,
		NULL
	);
	// 显示，更新窗口
	ShowWindow(windowId_, SW_SHOW);
	UpdateWindow(windowId_);
}



LRESULT CALLBACK windowCallback(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	//窗口回调函数
	// 窗口发生不同事件（按键/鼠标移动等）的时候本函数会自动调用
	// msg 代表了事件的类型，WM_DESTROY 表示窗口关闭
	// 在 windows 中，事件被称之为消息
	if (msg == WM_DESTROY) {
		PostQuitMessage(0);
		return 0;
	}
	else if (msg == WM_COMMAND) {
		int buttonId = (int)wp;
		//buttonAction(buttonId);
	}
	// 系统的默认窗口处理函数，这是套路
	return DefWindowProc(hWnd, msg, wp, lp);
}

void MainWindow::MyReisterClass() {

	// 设置窗口属性并初始化
	WNDCLASSEX	window = { 0 };
	// 这个是 WinMain 函数的第一个参数
	window.hInstance = instanceId_;   
	// 系统会在窗口事件发生后调用的函数
	window.lpfnWndProc = windowCallback;  
	// 这个要和下面 CreateWindow 第一个参数一样
	window.lpszClassName = className_;    

	// 下面的属性，不必具体了解，当做规定即可
	window.cbSize = sizeof(WNDCLASSEX);
	window.cbClsExtra = 0;
	window.cbWndExtra = 0;
	window.style = CS_CLASSDC;
	window.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	window.hCursor = LoadCursor(NULL, IDC_ARROW);
	window.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	window.lpszMenuName = NULL;
	window.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&window);
}

int MainWindow::runLoop() {
	// 创建 msg 变量并初始化
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	// 窗口进入消息循环
	// 如果窗口没有接收到退出消息的话，就一直循环
	while (msg.message != WM_QUIT) {
		// 获取消息，如果存在的话，执行 if 内的消息处理
		// 如果没有任何消息，GetMessage 就会等在这里
		if (GetMessage(&msg, NULL, 0, 0)) {
			// 这两行执行完后，系统就会自动调用 WndProc 函数
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}



void MainWindow::buttonAction(int buttonId) {
	if (buttonId == 0) {
		MessageBox(NULL, _T("1被按了"), _T("标题1"), MB_OK);
	}
	else if (buttonId == 1) {
		MessageBox(NULL, _T("2被按了"), _T("标题2"), MB_OK);
	}
}



HWND loadImage(HWND windowId) {
	const char* path = "al.kbcimage";
	FILE* imageFd;
	fopen_s(&imageFd, path, "rb");
	const int preTextSize = 3;
	char* imageInfo = new char[preTextSize];
	fread(imageInfo, sizeof(char), preTextSize, imageFd);
	int w = (unsigned char)imageInfo[1];
	int h = (unsigned char)imageInfo[2];
	stringstream s;
	s << string("image w and h[") << w << " " << h << string("]");
	CString content(s.str().c_str());
	MessageBox(NULL, content, _T("图像尺寸"), MB_OK);

	const int IMAGE_SIZE = w * h * 3;
	char* data = new char[IMAGE_SIZE];
	BITMAPINFOHEADER header;

	header.biSize = sizeof(BITMAPINFOHEADER);
	//必须是4的整倍数
	header.biWidth = w;
	header.biHeight = h;

	//指定表示颜色时要用到的位数，
	//常用的值为1(黑白二色图), 4(16色图), 8(256色), 24(真彩色图)(新的.bmp格式支持32位色，
	header.biBitCount = 3;
	//biCompression 指定位图是否压缩，有效的值为BI_RGB，BI_RLE8，BI_RLE4，BI_BITFIELDS(都是一些Windows定义好的常量
	header.biCompression = BI_RGB;
	// 指定实际的位图数据占用的字节数
	header.biSizeImage = 0;
	// 指定目标设备的垂直分辨率，单位同上。
	header.biXPelsPerMeter = 0;
	//定本图象实际用到的颜色数，如果该值为零，则用到的颜色数为2biBitCount。 
	header.biClrUsed = 0;
	//指定本图象中重要的颜色数，如果该值为零，则认为所有的颜色都是重要的。 
	header.biClrImportant = 0;

	//for (size_t y = 0; y < h; y++) {
	//	size_t offset = bytesPreRow * y;
	//	for (size_t x = 0; x < w; x++) {
	//		size_t index1 = y * 3 * w + x * 3;
	//		size_t index2 = offset + x * 4;
	//		pixels[index1 + 0] = bytes[index2 + 2];
	//		pixels[index1 + 1] = bytes[index2 + 1];
	//		pixels[index2 + 2] = bytes[index2 + 0];
	//	}
	//}

	//auto hdc = GetDC(windowId);
	//StretchDIBits(
	//	hdc,
	//	0, 0,
	//	header.biWidth,
	//	header.biHeight,
	//	0, h,
	//	header.biWidth,
	//	-header.biHeight,
	//	pixels,
	//	(BITMAPINFO*)&header,
	//	DIB_RGB_COLORS, SRCCOPY
	//);
	return windowId;
}


//loadImage(windowId);

//CreateWindow(
//	_T("BUTTON"),        
//	_T("按钮1"), // 窗口标题
//	 // 样式，分别是，创建一个子窗口，创建的窗口可见，按钮(类型)
//	WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
//	// 下面 4 个参数分别是窗口左上角的 x y 坐标以及窗口的 w h
//	0, 0, 400, 50,
//	windowId, // 按钮的父窗口，也就是放在哪个窗口里
//	(HMENU)0,           // 按钮的 ID，自行指定
//	hInstance,
//	NULL
//);
//CreateWindow(
//	_T("BUTTON"),
//	_T("Button2 标题"),
//	WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
//	0, 60, 400, 50,
//	windowId,
//	(HMENU)1,
//	hInstance,
//	NULL
//);