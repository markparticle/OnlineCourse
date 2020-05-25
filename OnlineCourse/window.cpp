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
	windowTitle_ = _T("�׿���");
	InitInstance();
	runLoop();
}

void MainWindow::InitInstance() {
	// ����������
	MyReisterClass();
	windowId_ = CreateWindow(
		className_,                  // ���Ҫ������� window �� lpszClassName ����һ��
		windowTitle_, // ���ڱ���
		WS_OVERLAPPEDWINDOW,        // ������ʽ
		// ���� 4 �������ֱ��Ǵ������Ͻǵ� x y �����Լ����ڵ� w h
		100, 100, 800, 600,
		// ���� 4 ������Ӧ�����涨
		NULL,
		NULL,
		instanceId_,
		NULL
	);
	// ��ʾ�����´���
	ShowWindow(windowId_, SW_SHOW);
	UpdateWindow(windowId_);
}



LRESULT CALLBACK windowCallback(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	//���ڻص�����
	// ���ڷ�����ͬ�¼�������/����ƶ��ȣ���ʱ�򱾺������Զ�����
	// msg �������¼������ͣ�WM_DESTROY ��ʾ���ڹر�
	// �� windows �У��¼�����֮Ϊ��Ϣ
	if (msg == WM_DESTROY) {
		PostQuitMessage(0);
		return 0;
	}
	else if (msg == WM_COMMAND) {
		int buttonId = (int)wp;
		//buttonAction(buttonId);
	}
	// ϵͳ��Ĭ�ϴ��ڴ�������������·
	return DefWindowProc(hWnd, msg, wp, lp);
}

void MainWindow::MyReisterClass() {

	// ���ô������Բ���ʼ��
	WNDCLASSEX	window = { 0 };
	// ����� WinMain �����ĵ�һ������
	window.hInstance = instanceId_;   
	// ϵͳ���ڴ����¼���������õĺ���
	window.lpfnWndProc = windowCallback;  
	// ���Ҫ������ CreateWindow ��һ������һ��
	window.lpszClassName = className_;    

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
	// ���ڽ�����Ϣѭ��
	// �������û�н��յ��˳���Ϣ�Ļ�����һֱѭ��
	while (msg.message != WM_QUIT) {
		// ��ȡ��Ϣ��������ڵĻ���ִ�� if �ڵ���Ϣ����
		// ���û���κ���Ϣ��GetMessage �ͻ��������
		if (GetMessage(&msg, NULL, 0, 0)) {
			// ������ִ�����ϵͳ�ͻ��Զ����� WndProc ����
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}



void MainWindow::buttonAction(int buttonId) {
	if (buttonId == 0) {
		MessageBox(NULL, _T("1������"), _T("����1"), MB_OK);
	}
	else if (buttonId == 1) {
		MessageBox(NULL, _T("2������"), _T("����2"), MB_OK);
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
	MessageBox(NULL, content, _T("ͼ��ߴ�"), MB_OK);

	const int IMAGE_SIZE = w * h * 3;
	char* data = new char[IMAGE_SIZE];
	BITMAPINFOHEADER header;

	header.biSize = sizeof(BITMAPINFOHEADER);
	//������4��������
	header.biWidth = w;
	header.biHeight = h;

	//ָ����ʾ��ɫʱҪ�õ���λ����
	//���õ�ֵΪ1(�ڰ׶�ɫͼ), 4(16ɫͼ), 8(256ɫ), 24(���ɫͼ)(�µ�.bmp��ʽ֧��32λɫ��
	header.biBitCount = 3;
	//biCompression ָ��λͼ�Ƿ�ѹ������Ч��ֵΪBI_RGB��BI_RLE8��BI_RLE4��BI_BITFIELDS(����һЩWindows����õĳ���
	header.biCompression = BI_RGB;
	// ָ��ʵ�ʵ�λͼ����ռ�õ��ֽ���
	header.biSizeImage = 0;
	// ָ��Ŀ���豸�Ĵ�ֱ�ֱ��ʣ���λͬ�ϡ�
	header.biXPelsPerMeter = 0;
	//����ͼ��ʵ���õ�����ɫ���������ֵΪ�㣬���õ�����ɫ��Ϊ2biBitCount�� 
	header.biClrUsed = 0;
	//ָ����ͼ������Ҫ����ɫ���������ֵΪ�㣬����Ϊ���е���ɫ������Ҫ�ġ� 
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
//	_T("��ť1"), // ���ڱ���
//	 // ��ʽ���ֱ��ǣ�����һ���Ӵ��ڣ������Ĵ��ڿɼ�����ť(����)
//	WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
//	// ���� 4 �������ֱ��Ǵ������Ͻǵ� x y �����Լ����ڵ� w h
//	0, 0, 400, 50,
//	windowId, // ��ť�ĸ����ڣ�Ҳ���Ƿ����ĸ�������
//	(HMENU)0,           // ��ť�� ID������ָ��
//	hInstance,
//	NULL
//);
//CreateWindow(
//	_T("BUTTON"),
//	_T("Button2 ����"),
//	WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
//	0, 60, 400, 50,
//	windowId,
//	(HMENU)1,
//	hInstance,
//	NULL
//);