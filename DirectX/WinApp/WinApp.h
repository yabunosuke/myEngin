#pragma once
#include <Windows.h>

class WinApp
{
public:
	static const wchar_t windowClassName[];
	static int windowWidth;  // ����
	static int windowHeight;  // �c��

public:		//�ÓI�����o�֐�
	//�E�B���h�E�v���\�W��
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public:		//�����o�֐�
	//�E�B���h�E����
	static void WindowCreate();
	//�E�B���h�E�j��
	static void WindowClassRelease();
	//Message����
	static bool WindowsMessage();
	//�n���h���擾
	static HWND GetHwnd() { return hwnd; };
	static HINSTANCE GetInstance() { return wndClass.hInstance; };
private:
	static HWND hwnd;
	static WNDCLASSEX wndClass;
};

