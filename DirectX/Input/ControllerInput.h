#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#include "WinApp.h"
#include <vector>

/*ゲームパッドのボタン入力*/
#define XBOX_INPUT_A      (0)  //Aボタン
#define XBOX_INPUT_B      (1)  //Bボタン
#define XBOX_INPUT_X      (2)  //Xボタン
#define XBOX_INPUT_Y      (3)  //Yボタン
#define XBOX_INPUT_LB     (4)  //LBボタン
#define XBOX_INPUT_RB     (5)  //RBボタン
#define XBOX_INPUT_SELECT (6)  //SELECTボタン
#define XBOX_INPUT_START  (7)  //STARTボタン
#define XBOX_INPUT_LSTICK (8)  //左スティック押し込み
#define XBOX_INPUT_RSTICK (9)  //右スティック押し込み
#define XBOX_INPUT_LEFT   (34) //十字キー左
#define XBOX_INPUT_RIGHT  (32) //十字キー右
#define XBOX_INPUT_UP     (35) //十字キー上
#define XBOX_INPUT_DOWN   (33) //十字キー下

/*ゲームパッドのスティック入力*/
#define INPUT_AXIS_X  (0x01) //左スティックのX軸
#define INPUT_AXIS_Y  (0x02) //左スティックのY軸
#define INPUT_AXIS_Z  (0x04) //左スティックのZ軸（LT・RT）
#define INPUT_AXIS_RX (0x08) //左スティックのX回転軸（右スティックのX軸）
#define INPUT_AXIS_RY (0x10) //左スティックのY回転軸（右スティックのY軸）
#define INPUT_AXIS_RZ (0x20) //左スティックのZ回転軸

class ControllerInput final
{
private: // デフォルトコンストラクタ・デストラクタのprivate化
	ControllerInput() {}
	~ControllerInput() {}
public: // コンストラクタ関係の設定
	// コピーコンストラクタの削除
	ControllerInput(const ControllerInput&) = delete;
	// 代入演算子の削除
	const ControllerInput& operator=(const ControllerInput&) = delete;
	// インスタンスの取得
	static ControllerInput* GetInstance();

private: // エイリアス
	// std::を省略
	template<class T> using vector = std::vector<T>;

public: // 定数
	static const LONG zoneMax; //スティック入力の上限値
	static const LONG zoneMin; //スティック入力の下限値

public: // 静的メンバ関数
	// 初期化
	static void Init();
	// 更新
	static void Update();

	// ゲームパッドの生の状態の取得
	static DIJOYSTATE GetGamePadState(const size_t& gamePadNo = 0);
	// ゲームパッドのボタンが押されているかどうか
	static bool IsPadButton(const size_t& button, const size_t& gamepadNo = 0);
	// ゲームパッドのボタンが押されている時間の取得
	static size_t GetPadButtonPress(const size_t& button, const size_t& gamepadNo = 0);
	// ゲームパッドのボタンを離している時間の取得
	static size_t GetPadButtonReleas(const size_t& button, const size_t& gamepadNo = 0);
	// ゲームパッドのボタンが押されたかどうか
	static bool IsPadButtonTrigger(const size_t& button, const size_t& gamepadNo = 0);
	// ゲームパッドのボタンから離れたかどうか
	static bool IsPadButtonReturn(const size_t& button, const size_t& gamepadNo = 0);
	// スティックの入力があるかどうか
	static int IsPadStick(const size_t& stickAxis, const float& deadzone, const size_t& gamepadNo = 0);
private:
	// ゲームパッドデバイスの生成
	static HRESULT CreateGamePadDevice();
	// デバイス列挙コールバック関数
	static BOOL CALLBACK EnumGamePadCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);
	// プロパティの設定
	static HRESULT SetUpGamePadProperty(const size_t& gamepadNo = 0);

private: //静的メンバ変数
	static IDirectInput8* dinput; //KeyboardInputのInitialize()内にあるdinputと統一した方がメモリ効率がいい
	static vector<IDirectInputDevice8*> devGamepad; //ゲームパッドデバイス
	static vector<DIJOYSTATE> gamepad; //ゲームパッドの生の状態
	static vector<DIJOYSTATE> oldgamepad; //ゲームパッドの生の状態
	static size_t pressCount[64]; //押している時間
	static size_t releasCount[64]; //離している時間

};
