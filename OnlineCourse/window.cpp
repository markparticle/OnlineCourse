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
	MessageBox(NULL, content, _T("ͼ��ߴ�"), MB_OK);

	const int IMAGE_SIZE = w * h * 3;
	char* data = new char[IMAGE_SIZE];
	fread(data, 1, w * h * 3, imageFd);

	BITMAPINFOHEADER header;

	header.biSize = sizeof(BITMAPINFOHEADER);
	//������4��������
	header.biWidth = w;
	header.biHeight = h;
	header.biPlanes = 1;
	//ָ����ʾ��ɫʱҪ�õ���λ����
	//���õ�ֵΪ1(�ڰ׶�ɫͼ), 4(16ɫͼ), 8(256ɫ), 24(���ɫͼ)(�µ�.bmp��ʽ֧��32λɫ��
	header.biBitCount = 32;
	//biCompression ָ��λͼ�Ƿ�ѹ������Ч��ֵΪBI_RGB��BI_RLE8��BI_RLE4��BI_BITFIELDS(����һЩWindows����õĳ���
	header.biCompression = BI_RGB;
	// ָ��ʵ�ʵ�λͼ����ռ�õ��ֽ���
	header.biSizeImage = 0;
	// ָ��Ŀ���豸�Ĵ�ֱ�ֱ���
	header.biXPelsPerMeter = 0;
	//����ͼ��ʵ���õ�����ɫ���������ֵΪ�㣬���õ�����ɫ��Ϊ2biBitCount�� 
	header.biClrUsed = 0;
	//ָ����ͼ������Ҫ����ɫ���������ֵΪ�㣬����Ϊ���е���ɫ������Ҫ�ġ� 
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
	windowTitle_ = _T("�׿���");
	MyReisterClass();
	InitInstance();

	loadImage(windowId_);
	CreateWindow(
		_T("Edit"), 
		_T(""),   
		// ��ʽ�趨���Ӵ��ڣ����ڿɼ��������������������������������б༭��
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
		// ��ʽ�趨���Ӵ��ڣ����ڿɼ��������������������������������б༭��
		WS_CHILD | WS_VISIBLE | WS_VSCROLL,
		300, 300, 300, 100,
		windowId_,
		(HMENU)UserPasswordInputTag,
		instanceId_,
		NULL
	);

	CreateWindow(
		_T("BUTTON"),
		_T("��¼"),
		//����һ���Ӵ��ڣ������Ĵ��ڿɼ�������(����)
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
	// ����������
	windowId_ = CreateWindow(
		className_,           
		// ���ڱ���
		windowTitle_, 
		// ������ʽ
		WS_OVERLAPPEDWINDOW,       
		// ���� 4 �������ֱ��Ǵ������Ͻǵ� x y �����Լ����ڵ� w h
		100, 100, 800, 600,
		// ���� 4 ������Ӧ�����涨
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
	content.Format(_T("����[ %c ]������"), char(key));
	MessageBox(NULL, content, _T("�����¼�"), MB_OK);
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
			MessageBoxW(NULL, _T("��¼�ɹ�"), _T("ϵͳ��ʾ"), MB_OK);
		}
		else 
		{
			content.Format(_T("%s%s"), userText, pwdText);
			MessageBoxW(NULL, content, _T("��������"), MB_OK); 
		}
	}
}

LRESULT CALLBACK windowCallback(HWND windowId, UINT msg, WPARAM wp, LPARAM lp) {
	//���ڻص�����
	//���ڷ�����ͬ�¼�������/����ƶ��ȣ���ʱ�򱾺������Զ�����
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
		//// TODO: �ڴ˴����ʹ�� hdc ���κλ�ͼ����...
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

	// ���ô������Բ���ʼ��
	WNDCLASSEX	window = { 0 };

	window.hInstance = instanceId_;   
	window.lpszClassName = className_; 
	// ϵͳ���ڴ����¼���������õĺ���
	window.lpfnWndProc = windowCallback;

	// ��������ԣ����ؾ����˽⣬�����涨����
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
	// ���� msg ��������ʼ��
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT) {
		// ���û���κ���Ϣ��GetMessage �ͻ��������
		if (GetMessage(&msg, NULL, 0, 0)) {
			// ������ִ�����ϵͳ�ͻ��Զ����� WndProc ����
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}

