#pragma once
#include <Windows.h>

class WinApp
{
public:
	static const wchar_t windowClassName[];
	static int windowWidth;  // 横幅
	static int windowHeight;  // 縦幅

public:		//静的メンバ関数
	//ウィンドウプロ―ジャ
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public:		//メンバ関数
	//ウィンドウ生成
	static void WindowCreate();
	//ウィンドウ破棄
	static void WindowClassRelease();
	//Message処理
	static bool WindowsMessage();
	//ハンドル取得
	static HWND GetHwnd() { return hwnd; };
	static HINSTANCE GetInstance() { return wndClass.hInstance; };
private:
	static HWND hwnd;
	static WNDCLASSEX wndClass;
};

