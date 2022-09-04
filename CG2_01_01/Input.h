#pragma once

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#include "Property.h"


class Input
{

public:


	//===========================================
	//
	//		静的変数
	//
	//===========================================

	// キーボード

	//===========================================
	//
	//		静的関数
	//
	//===========================================

	static void Initialize();		// 初期化
	static void Update();			// 更新




	// キーボード

	/// <summary>
	/// キーを入力しているフレーム数を返す
	/// </summary>
	/// <returns>int 入力フレーム数</returns>
	static int GetKeyPress(int key);

	/// <summary>
	/// キーが押された瞬間だけtrueになる
	/// </summary>
	/// <returns>bool 押された瞬間</returns>
	static bool GetKeyPressTrigger(int key);

	/// <summary>
	/// キーを離しているフレーム数を返す
	/// </summary>
	/// <returns>int 入力フレーム数</returns>
	static int GetKeyReleas(int key);

	/// <summary>
	/// キーが離された瞬間だけtrueになる
	/// </summary>
	/// <returns>bool 押された瞬間</returns>
	static bool GetKeyReleasTrigger(int key);

	// コントローラー




private:

	Input(){};
	~Input(){};



	// コントローラー

	// デバイス列挙コールバック関数
	static BOOL CALLBACK EnumGamePadCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);
	// プロパティセット
	static HRESULT SetUpGamePadProperty(const size_t &gamepad_no = 0);

	// 共通
	static bool any_key_;			// いずれかのキーかマウスが押されていればtrue

	// キーボード
	static int key_press_count_[256];	// キー長押し確認用
	static int key_releas_count_[256];	// キー手放し確認用

	// コントローラー
	static int controller_press_count_[64];		// コントローラー長押し確認用
	static int controller_releas_count_[64];	// コントローラー手放し確認用
	static const LONG zone_max;					// スティック入力の上限
	static const LONG zone_min;					// スティック入力の下限
};

