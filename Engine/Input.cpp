#include "Input.h"

#include "WinAPP.h"
#include <vector>

bool Input::any_key_ = false;

IDirectInputDevice8 *devkeyboard = nullptr;
int Input::key_press_count_[256] = { 0 };
int Input::key_releas_count_[256] = { 0 };

std::vector<IDirectInputDevice8*> dev_gamepad;
int Input::controller_press_count_[64] = { 0 };
int Input::controller_releas_count_[64] = { 0 };
const LONG Input::zone_max = +1000;
const LONG Input::zone_min = -1000;


void Input::Initialize()
{
	HRESULT result;
	IDirectInput8 *dinput = nullptr;

	result = DirectInput8Create(
		WinApp::GetInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&dinput, nullptr);

	// キーボード初期化
	{
		result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);
		result = devkeyboard->SetDataFormat(&c_dfDIKeyboard);   //標準形式
		result = devkeyboard->SetCooperativeLevel(
			WinApp::GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	}

	// コントローラー初期化
	{
		static std::vector<DIDEVICEINSTANCE> parameter;
		result = dinput->EnumDevices(DI8DEVTYPE_GAMEPAD, &EnumGamePadCallback, &parameter, DIEDFL_ATTACHEDONLY);
		for (size_t i = 0; i < parameter.size(); i++)
		{
			dev_gamepad.push_back({});

			// ゲームパッドデバイスの生成
			result = dinput->CreateDevice(parameter[i].guidInstance, &dev_gamepad[i], NULL);
			// 入力データ形式のセット
			result = dev_gamepad[i]->SetDataFormat(&c_dfDIJoystick);
			// プロパティの設定
			result = SetUpGamePadProperty(i);
			// 協調レベルの設定
			result = dev_gamepad[i]->SetCooperativeLevel(WinApp::GetHwnd(), DISCL_FOREGROUND | DISCL_EXCLUSIVE);
		}

	}


}

void Input::Update()
{
	any_key_ = false;
	HRESULT result;
    // キーボード更新
	{
		result = devkeyboard->Acquire();
		BYTE key[256] = {};
		result = devkeyboard->GetDeviceState(sizeof(key), key);

		// 全キーをチェック
		for (int i = 0; i < 256; i++) 
		{

			if (key[i] != 0) 
			{
				if (key_releas_count_[i] != 0) 
				{
					key_releas_count_[i] = 0;
				}
				key_press_count_[i]++;

				// キー入力があるのでtrueに変更
				any_key_ = true;
			}
			else 
			{
				if (key_press_count_[i] != 0) 
				{
					key_press_count_[i] = 0;
				}
				key_releas_count_[i]++;
			}
		}
	}

	// コントローラー
	{
		if(dev_gamepad.size() == 0)
		{
			
		}
		for (size_t i = 0; i < dev_gamepad.size(); i++)
		{
			result = dev_gamepad[i]->Acquire();
			if (result == DIERR_INPUTLOST)
			{
				// デバイスがロストしていたら無視する
				continue;
			}

			dev_gamepad[i]->Poll();

			//if (gamepad.size() < dev_gamepad.size())
			//{
			//	do
			//	{
			//		// 数合わせ
			//		gamepad.push_back({});
			//		oldgamepad.push_back({});
			//	} while (gamepad.size() != devGamepad.size());
			//}

			//oldgamepad[i] = gamepad[i];

			//ZeroMemory(&gamepad[i], sizeof(DIJOYSTATE));
			//dev_gamepad[i]->GetDeviceState(sizeof(DIJOYSTATE), &gamepad[i]);
		}
	}
}

int Input::GetKeyPress(int key)
{
	return key_press_count_[key];
}

bool Input::GetKeyPressTrigger(int key)
{
	return key_press_count_[key] == 1;
}

int Input::GetKeyReleas(int key)
{
	return key_releas_count_[key];
}

bool Input::GetKeyReleasTrigger(int key)
{
	return key_releas_count_[key] == 1;
}

BOOL Input::EnumGamePadCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
{
	std::vector<DIDEVICEINSTANCE> *ptr = (std::vector<DIDEVICEINSTANCE>*)pvRef;
	DIDEVICEINSTANCE tmp = *lpddi;

	// 配列に格納
	ptr->push_back(tmp);

	return DIENUM_CONTINUE;
}

HRESULT Input::SetUpGamePadProperty(const size_t& gamepad_no)
{
	if (gamepad_no >= dev_gamepad.size())
	{
		return S_FALSE;
	}


	HRESULT hr;

	// 軸モードを絶対値モードとして設定
	DIPROPDWORD diprop;
	ZeroMemory(&diprop, sizeof(diprop));
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.diph.dwObj = 0;
	diprop.dwData = DIPROPAXISMODE_ABS;
	hr = dev_gamepad[gamepad_no]->SetProperty(DIPROP_AXISMODE, &diprop.diph);
	if (FAILED(hr))
	{
		return hr;
	}

	// X軸の値の範囲設定
	DIPROPRANGE diprg;
	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.diph.dwObj = DIJOFS_X;
	diprg.lMin = zone_min;
	diprg.lMax = zone_max;
	hr = dev_gamepad[gamepad_no]->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(hr))
	{
		return hr;
	}

	// Y軸の値の範囲設定
	diprg.diph.dwObj = DIJOFS_Y;
	hr = dev_gamepad[gamepad_no]->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(hr))
	{
		return hr;
	}

	// Z軸の値の範囲設定
	diprg.diph.dwObj = DIJOFS_Z;
	hr = dev_gamepad[gamepad_no]->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(hr))
	{
		return hr;
	}

	// RX軸の値の範囲設定
	diprg.diph.dwObj = DIJOFS_RX;
	hr = dev_gamepad[gamepad_no]->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(hr))
	{
		return hr;
	}

	// RY軸の値の範囲設定
	diprg.diph.dwObj = DIJOFS_RY;
	hr = dev_gamepad[gamepad_no]->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(hr))
	{
		return hr;
	}

	// RZ軸の値の範囲設定
	diprg.diph.dwObj = DIJOFS_RZ;
	hr = dev_gamepad[gamepad_no]->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(hr))
	{
		return hr;
	}

	return S_OK;
}
