#include "Input.h"

#include "WinAPP.h"
#include <vector>

bool Input::any_key_ = false;

IDirectInputDevice8 *devkeyboard = nullptr;
int Input::key_press_count_[256] = { 0 };
int Input::key_releas_count_[256] = { 0 };

std::vector<IDirectInputDevice8*> Input::dev_gamepads_;
std::vector<DIJOYSTATE> Input::gamepads_ = {};
int Input::controller_press_count_[64] = { 0 };
int Input::controller_releas_count_[64] = { 0 };
const LONG Input::zone_max_ = +1000;
const LONG Input::zone_min_ = -1000;


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
			dev_gamepads_.push_back({});

			// ゲームパッドデバイスの生成
			result = dinput->CreateDevice(parameter[i].guidInstance, &dev_gamepads_[i], NULL);
			// 入力データ形式のセット
			result = dev_gamepads_[i]->SetDataFormat(&c_dfDIJoystick);
			// プロパティの設定
			result = SetUpGamePadProperty(i);
			// 協調レベルの設定
			result = dev_gamepads_[i]->SetCooperativeLevel(WinApp::GetHwnd(), DISCL_FOREGROUND | DISCL_EXCLUSIVE);
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

	// コントローラー更新
	ControllerUpdate();
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

int Input::GetButtonPress(GamePadButton button)
{
	return controller_press_count_[static_cast<int>(button)];
}

bool Input::GetButtonPressTrigger(GamePadButton button)
{
	return controller_press_count_[static_cast<int>(button)] == 1;
}

int Input::GetButtonReleas(GamePadButton button)
{
	return controller_releas_count_[static_cast<int>(button)];
}

bool Input::GetButtonReleasTrigger(GamePadButton button)
{
	return controller_releas_count_[static_cast<int>(button)] == 1;
}

Vector2 Input::GetStick(GamePadStick stick, const float &dead_zone, const size_t &gamepad_no)
{
	Vector2 input_vec;
	switch (stick)
	{
	case GamePadStick::Stick_L:
		input_vec =
		{
			GetAxis(GamePadAxis::AXIS_LX,dead_zone,gamepad_no),
			GetAxis(GamePadAxis::AXIS_LY,dead_zone,gamepad_no)
		};
		break;
	case GamePadStick::Stick_R:
		input_vec =
		{
			GetAxis(GamePadAxis::AXIS_RX,dead_zone,gamepad_no),
			GetAxis(GamePadAxis::AXIS_RY,dead_zone,gamepad_no)
		};
		break;
	default:
		input_vec =
		{
			0.0f,
			0.0f
		};
		break;
	}
	return input_vec;
}

float Input::GetAxis(GamePadAxis axis, const float &dead_zone, const size_t &gamepad_no)
{

	int dead_min = zone_min_ <= 0 ? (int)(zone_min_ * dead_zone) : (int)(zone_min_ * dead_zone) + zone_min_;
	int dead_max = zone_max_ >= 0 ? (int)(zone_max_ * dead_zone) : (int)(zone_max_ * dead_zone) + zone_max_;

	int stick = 0;
	switch (axis)
	{
	case GamePadAxis::AXIS_LX:
		stick = gamepads_[gamepad_no].lX;
		break;
	case GamePadAxis::AXIS_LY:
		stick = gamepads_[gamepad_no].lY;
		break;
	case GamePadAxis::AXIS_LZ:
		stick = gamepads_[gamepad_no].lZ;
		break;
	case GamePadAxis::AXIS_RX:
		stick = gamepads_[gamepad_no].lRx;
		break;
	case GamePadAxis::AXIS_RY:
		stick = gamepads_[gamepad_no].lRy;
		break;
	case GamePadAxis::AXIS_RZ:
		stick = gamepads_[gamepad_no].lRz;
		break;
	default:
		stick = 0;
		break;
	}
	// デッドゾーンの処理

	if ((stick <= dead_min && stick < 0) ||
		(stick >= dead_max && stick > 0))
	{
		return static_cast<float>(stick) / static_cast<float>(zone_max_);
	}
	else
	{
		return 0.0f;
	}
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
	if (gamepad_no >= dev_gamepads_.size())
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
	hr = dev_gamepads_[gamepad_no]->SetProperty(DIPROP_AXISMODE, &diprop.diph);
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
	diprg.lMin = zone_min_;
	diprg.lMax = zone_max_;
	hr = dev_gamepads_[gamepad_no]->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(hr))
	{
		return hr;
	}

	// Y軸の値の範囲設定
	diprg.diph.dwObj = DIJOFS_Y;
	hr = dev_gamepads_[gamepad_no]->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(hr))
	{
		return hr;
	}

	// Z軸の値の範囲設定
	diprg.diph.dwObj = DIJOFS_Z;
	hr = dev_gamepads_[gamepad_no]->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(hr))
	{
		return hr;
	}

	// RX軸の値の範囲設定
	diprg.diph.dwObj = DIJOFS_RX;
	hr = dev_gamepads_[gamepad_no]->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(hr))
	{
		return hr;
	}

	// RY軸の値の範囲設定
	diprg.diph.dwObj = DIJOFS_RY;
	hr = dev_gamepads_[gamepad_no]->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(hr))
	{
		return hr;
	}

	// RZ軸の値の範囲設定
	diprg.diph.dwObj = DIJOFS_RZ;
	hr = dev_gamepads_[gamepad_no]->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(hr))
	{
		return hr;
	}

	return S_OK;
}

void Input::ControllerUpdate()
{
	HRESULT result;
	if (dev_gamepads_.size() == 0)
	{
		return;
	}

	for (size_t i = 0; i < dev_gamepads_.size(); i++)
	{
		result = dev_gamepads_[i]->Acquire();
		if (result == DIERR_INPUTLOST)
		{
			// デバイスがロストしていたら無視する
			continue;
		}

		dev_gamepads_[i]->Poll();

		if (gamepads_.size() < dev_gamepads_.size())
		{
			do
			{
				// 数合わせ
				gamepads_.push_back({});
			} while (gamepads_.size() != dev_gamepads_.size());
		}

		ZeroMemory(&gamepads_[i], sizeof(DIJOYSTATE));
		dev_gamepads_[i]->GetDeviceState(sizeof(DIJOYSTATE), &gamepads_[i]);
	}

	static const size_t button_count = sizeof(gamepads_[0].rgbButtons) / sizeof(gamepads_[0].rgbButtons[0]);
	bool any_button = false;	//何らかの入力があるか

	for (size_t i = 0; i < 64; i++)
	{
		if (i < button_count)
		{
			// 押されている間
			if (gamepads_[0].rgbButtons[i])
			{
				if (controller_releas_count_[i] != 0)
				{
					controller_releas_count_[i] = 0;
				}

				controller_press_count_[i]++;
			}
			// 離されている間
			else
			{
				if (controller_press_count_[i] != 0)
				{
					controller_press_count_[i] = 0;
				}

				controller_releas_count_[i]++;
			}
		}
		else if (i == button_count + 4)
		{
			if (i == static_cast<int>(GamePadButton::INPUT_LEFT))
			{
				any_button =
					gamepads_[0].rgdwPOV[0] == 4500 * 5 ||
					gamepads_[0].rgdwPOV[0] == 4500 * 6 ||
					gamepads_[0].rgdwPOV[0] == 4500 * 7;
			}
			else if (i == static_cast<int>(GamePadButton::INPUT_RIGHT))
			{
				any_button =
					gamepads_[0].rgdwPOV[0] == 4500 * 1 ||
					gamepads_[0].rgdwPOV[0] == 4500 * 2 ||
					gamepads_[0].rgdwPOV[0] == 4500 * 3;
			}
			else if (i == static_cast<int>(GamePadButton::INPUT_UP))
			{
				any_button =
					gamepads_[0].rgdwPOV[0] == 4500 * 7 ||
					gamepads_[0].rgdwPOV[0] == 4500 * 0 ||
					gamepads_[0].rgdwPOV[0] == 4500 * 1;
			}
			else if (i == static_cast<int>(GamePadButton::INPUT_DOWN))
			{
				any_button =
					gamepads_[0].rgdwPOV[0] == 4500 * 3 ||
					gamepads_[0].rgdwPOV[0] == 4500 * 4 ||
					gamepads_[0].rgdwPOV[0] == 4500 * 5;
			}

			if (any_button)
			{
				if (controller_releas_count_[i] != 0)
				{
					controller_releas_count_[i] = 0;
				}

				controller_press_count_[i]++;
			}
			else
			{
				if (controller_press_count_[i] != 0)
				{
					controller_press_count_[i] = 0;
				}

				controller_releas_count_[i]++;
			}
		}
		else
		{
			if (controller_press_count_[i] != 0)
			{
				controller_press_count_[i] = 0;
			}

			controller_releas_count_[i]++;
		}
	}
}