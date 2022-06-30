#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#include "WinApp.h"

class KeyboardInput final
{
private:	//シングルトンパターン
	KeyboardInput() = default;
	~KeyboardInput() = default;
	KeyboardInput(const KeyboardInput & r) = default;
	KeyboardInput &operator= (const KeyboardInput & r) = default;
public:		//インスタンスの取得
	static KeyboardInput *GetIns();

public:
	void Initialize();
	void Update();
	
	bool GetKeyPressT(int key);
	bool GetKeyReleasT(int key);
	int GetKeyPress(int key);
	int GetKeyReleas(int key);

private:
	static int pressCount[256];
	static int releasCount[256];
};

