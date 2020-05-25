/*
 * @Author       : mark
 * @Date         : 2020-05-24
 * @copyleft GPL 2.0
 */
#include <iostream>
#include "window.h"

using std::string;
using std::stringstream;

void GetOffset(string flags,  int* offset) {
	if (flags == "RGB") {
		offset[0] = 0;
		offset[1] = 1;
		offset[2] = 2;
	}  
	else if (flags == "BGR") {
		offset[0] = 2;
		offset[1] = 1;
		offset[2] = 0;
	}
	else if (flags == "BRG") {
		offset[0] = 2;
		offset[1] = 0;
		offset[2] = 1;
	}
}

HWND loadImage(HWND windowId) {
	const char* path = "resourse\\a2.bgr.kbcimage";
	FILE* imageFd;
	fopen_s(&imageFd, path, "rb");
	fseek(imageFd, 0, SEEK_SET);

	const int headSize = 3;
	char* imageInfo = new char[headSize];
	fread(imageInfo, 1, headSize, imageFd);
	int w = (unsigned char)imageInfo[1];
	int h = (unsigned char)imageInfo[2];
	CString content;
	content.Format(_T("image w and h [%d %d] "), w, h);
	MessageBox(NULL, content, _T("图像尺寸"), MB_OK);

	const int IMAGE_SIZE = w * h * 3;
	char* data = new char[IMAGE_SIZE];
	fread(data, 1, w * h * 3, imageFd);

	BITMAPINFOHEADER header;

	header.biSize = sizeof(BITMAPINFOHEADER);
	//必须是4的整倍数
	header.biWidth = w;
	header.biHeight = h;
	header.biPlanes = 1;
	//指定表示颜色时要用到的位数，
	//常用的值为1(黑白二色图), 4(16色图), 8(256色), 24(真彩色图)(新的.bmp格式支持32位色，
	header.biBitCount = 32;
	//biCompression 指定位图是否压缩，有效的值为BI_RGB，BI_RLE8，BI_RLE4，BI_BITFIELDS(都是一些Windows定义好的常量
	header.biCompression = BI_RGB;
	// 指定实际的位图数据占用的字节数
	header.biSizeImage = 0;
	// 指定目标设备的垂直分辨率
	header.biXPelsPerMeter = 0;
	//定本图象实际用到的颜色数，如果该值为零，则用到的颜色数为2biBitCount。 
	header.biClrUsed = 0;
	//指定本图象中重要的颜色数，如果该值为零，则认为所有的颜色都是重要的。 
	header.biClrImportant = 0;

	RGBQUAD* pixels = new RGBQUAD[IMAGE_SIZE];
	int offset[3];
	GetOffset("BGR", offset);
	for (size_t i = 0; i < w * h; i++) {
		pixels[i].rgbRed = data[3 * i + offset[0]];
		pixels[i].rgbGreen = data[3 * i + offset[1]];
		pixels[i].rgbBlue = data[3 * i + offset[2]];
		pixels[i].rgbReserved = 0;
	}
	auto hdc = GetDC(windowId);
	StretchDIBits(
		hdc,
		0, 0,
		header.biWidth,
		header.biHeight,
		0, h,
		header.biWidth,
		-header.biHeight,
		pixels,
		(BITMAPINFO*)&header,
		DIB_RGB_COLORS, SRCCOPY
	);
	return windowId;
}


MainWindow::MainWindow(HINSTANCE hInstance): instanceId_(hInstance){
	className_ = _T("WindowClass");
	windowTitle_ = _T("雷课堂");
	MyReisterClass();
	InitInstance();

	loadImage(windowId_);
	CreateWindow(
		_T("Edit"), 
		_T(""),   
		// 样式设定，子窗口，窗口可见，带纵向滚动条，带横向滚动条，多行编辑框，
		//WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE,
		WS_CHILD | WS_VISIBLE | WS_VSCROLL,
		300, 150, 300, 100,
		windowId_,
		(HMENU)UserNameInputTag,
		instanceId_,
		NULL
	);

	CreateWindow(
		_T("Edit"),
		_T(""),
		// 样式设定，子窗口，窗口可见，带纵向滚动条，带横向滚动条，多行编辑框，
		WS_CHILD | WS_VISIBLE | WS_VSCROLL,
		300, 300, 300, 100,
		windowId_,
		(HMENU)UserPasswordInputTag,
		instanceId_,
		NULL
	);

	CreateWindow(
		_T("BUTTON"),
		_T("登录"),
		//创建一个子窗口，创建的窗口可见，按键(类型)
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		300, 400, 100, 100,
		windowId_,
		(HMENU)LoginButtonTag,
		instanceId_,
		NULL
	);
	runLoop();
}

void MainWindow::InitInstance() {
	// 创建主窗口
	windowId_ = CreateWindow(
		className_,           
		// 窗口标题
		windowTitle_, 
		// 窗口样式
		WS_OVERLAPPEDWINDOW,       
		// 下面 4 个参数分别是窗口左上角的 x y 坐标以及窗口的 w h
		100, 100, 800, 600,
		// 下面 4 个现在应当做规定
		NULL,
		NULL,
		instanceId_,
		NULL
	);
	ShowWindow(windowId_, SW_SHOW);
	UpdateWindow(windowId_);
}


void KeyboardAction(unsigned char key) {
	CString content;
	content.Format(_T("按键[ %c ]被按下"), char(key));
	MessageBox(NULL, content, _T("按键事件"), MB_OK);
}


void ButtonAction(int buttonTag, HWND windowId) 
{
	if (buttonTag == LoginButtonTag) 
	{
		HWND inputId;
		const int length = 128;
		TCHAR userText[length];
		TCHAR pwdText[length];
		CString content;

		inputId = GetDlgItem(windowId, UserNameInputTag);
		GetWindowText(inputId, userText, length);
	
		inputId = GetDlgItem(windowId, UserPasswordInputTag);
		GetWindowText(inputId, pwdText, length);

		if (_tcscmp(userText,_T("admin")) == 0 && _tcscmp(pwdText,_T("123")) == 0)
		{
			MessageBoxW(NULL, _T("登录成功"), _T("系统提示"), MB_OK);
		}
		else 
		{
			content.Format(_T("%s%s"), userText, pwdText);
			MessageBoxW(NULL, content, _T("输入内容"), MB_OK); 
		}
	}
}

LRESULT CALLBACK windowCallback(HWND windowId, UINT msg, WPARAM wp, LPARAM lp) {
	//窗口回调函数
	//窗口发生不同事件（按键/鼠标移动等）的时候本函数会自动调用
	switch (msg)
	{
	case WM_COMMAND:
	{

		int buttonId = (int)wp;
		ButtonAction(buttonId, windowId);
	}
	break;
	case WM_KEYDOWN:
	{
		unsigned char key = (unsigned char)wp;
		KeyboardAction(key);
	}
	break;
	//case WM_PAINT:
	//{
		//PAINTSTRUCT ps;
		//HDC hdc = BeginPaint(hWnd, &ps);
		//// TODO: 在此处添加使用 hdc 的任何绘图代码...
		//EndPaint(hWnd, &ps);
	//}
	//break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(windowId, msg, wp, lp);
	}
	return 0;
}

void MainWindow::MyReisterClass() {

	// 设置窗口属性并初始化
	WNDCLASSEX	window = { 0 };

	window.hInstance = instanceId_;   
	window.lpszClassName = className_; 
	// 系统会在窗口事件发生后调用的函数
	window.lpfnWndProc = windowCallback;

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
	while (msg.message != WM_QUIT) {
		// 如果没有任何消息，GetMessage 就会等在这里
		if (GetMessage(&msg, NULL, 0, 0)) {
			// 这两行执行完后，系统就会自动调用 WndProc 函数
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}

