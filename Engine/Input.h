#pragma once

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#include "Property.h"
#include "Math/Vector2.h"


//===========================================
//
//		サブクラス
//
//===========================================

// コントローラーのボタン
enum class GamePadButton
{
	INPUT_A,			//Aボタン
	INPUT_B,			//Bボタン
	INPUT_X,			//Xボタン
	INPUT_Y,			//Yボタン
	INPUT_LB,			//LBボタン
	INPUT_RB,			//RBボタン
	INPUT_SELECT,		//SELECTボタン
	INPUT_START,		//STARTボタン
	INPUT_LSTICK,		//左スティック押し込み
	INPUT_RSTICK,		//右スティック押し込み
	INPUT_LEFT = 34,	//十字キー左
	INPUT_RIGHT = 32,	//十字キー右
	INPUT_UP = 35,	//十字キー上
	INPUT_DOWN = 33,	//十字キー下
};
// コントローラーのスティック
enum class GamePadStick
{
	Stick_L,
	Stick_R,
};

enum class GamePadAxis
{
	AXIS_LX = 0x01,			//左スティックX軸
	AXIS_LY = 0x02,			//左スティックY軸
	AXIS_LZ = 0x04,			//左スティックZ軸(LT・RT)
	AXIS_RX = 0x08,			//右スティックX軸
	AXIS_RY = 0x10,			//右スティックY軸
	AXIS_RZ = 0x20,			//右スティックZ軸
};

// 入力の更新はFixedUpdateのタイミングで行われる
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
	
	/// <summary>
	/// ボタンを入力しているフレーム数を返す
	/// </summary>
	/// <returns>int 入力フレーム数</returns>
	static int GetButtonPress(GamePadButton button);

	/// <summary>
	/// ボタンが押された瞬間だけtrueになる
	/// </summary>
	/// <returns>bool 押された瞬間</returns>
	static bool GetButtonPressTrigger(GamePadButton button);

	/// <summary>
	/// ボタンを離されているフレーム数を返す
	/// </summary>
	/// <returns>int 未入力フレーム数</returns>
	static int GetButtonReleas(GamePadButton button);

	/// <summary>
	/// ボタンが離された瞬間だけtrueになる
	/// </summary>
	/// <returns>bool 離された瞬間</returns>
	static bool GetButtonReleasTrigger(GamePadButton button);

	/// <summary>
	/// スティックの入力状態を返す
	/// </summary>
	/// <returns>Vector2 入力角度</returns>
	static Vector2 GetStick(GamePadStick stick, const float &dead_zone = 0.2f, const size_t &gamepad_no = 0);

	/// <summary>
	/// 各軸の入力を返す
	/// </summary>
	/// <returns>float 軸の傾き度合(0.0~1.0)</returns>
	static float GetAxis(GamePadAxis axis,const float &dead_zone = 0.2f,const size_t& gamepad_no = 0);


private:

	Input(){};
	~Input(){};




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
	static void ControllerUpdate();
	static std::vector<IDirectInputDevice8 *> dev_gamepads_; //ゲームパッドデバイス
	static std::vector<DIJOYSTATE> gamepads_; //ゲームパッドの生の状態
	static int controller_press_count_[64];		// コントローラー長押し確認用
	static int controller_releas_count_[64];	// コントローラー手放し確認用
	static const LONG zone_max_;					// スティック入力の上限
	static const LONG zone_min_;					// スティック入力の下限
};

