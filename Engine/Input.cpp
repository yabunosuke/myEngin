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

	// �L�[�{�[�h������
	{
		result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);
		result = devkeyboard->SetDataFormat(&c_dfDIKeyboard);   //�W���`��
		result = devkeyboard->SetCooperativeLevel(
			WinApp::GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	}

	// �R���g���[���[������
	{
		static std::vector<DIDEVICEINSTANCE> parameter;
		result = dinput->EnumDevices(DI8DEVTYPE_GAMEPAD, &EnumGamePadCallback, &parameter, DIEDFL_ATTACHEDONLY);
		for (size_t i = 0; i < parameter.size(); i++)
		{
			dev_gamepad.push_back({});

			// �Q�[���p�b�h�f�o�C�X�̐���
			result = dinput->CreateDevice(parameter[i].guidInstance, &dev_gamepad[i], NULL);
			// ���̓f�[�^�`���̃Z�b�g
			result = dev_gamepad[i]->SetDataFormat(&c_dfDIJoystick);
			// �v���p�e�B�̐ݒ�
			result = SetUpGamePadProperty(i);
			// �������x���̐ݒ�
			result = dev_gamepad[i]->SetCooperativeLevel(WinApp::GetHwnd(), DISCL_FOREGROUND | DISCL_EXCLUSIVE);
		}

	}


}

void Input::Update()
{
	any_key_ = false;
	HRESULT result;
    // �L�[�{�[�h�X�V
	{
		result = devkeyboard->Acquire();
		BYTE key[256] = {};
		result = devkeyboard->GetDeviceState(sizeof(key), key);

		// �S�L�[���`�F�b�N
		for (int i = 0; i < 256; i++) 
		{

			if (key[i] != 0) 
			{
				if (key_releas_count_[i] != 0) 
				{
					key_releas_count_[i] = 0;
				}
				key_press_count_[i]++;

				// �L�[���͂�����̂�true�ɕύX
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

	// �R���g���[���[
	{
		if(dev_gamepad.size() == 0)
		{
			
		}
		for (size_t i = 0; i < dev_gamepad.size(); i++)
		{
			result = dev_gamepad[i]->Acquire();
			if (result == DIERR_INPUTLOST)
			{
				// �f�o�C�X�����X�g���Ă����疳������
				continue;
			}

			dev_gamepad[i]->Poll();

			//if (gamepad.size() < dev_gamepad.size())
			//{
			//	do
			//	{
			//		// �����킹
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

	// �z��Ɋi�[
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

	// �����[�h���Βl���[�h�Ƃ��Đݒ�
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

	// X���̒l�͈̔͐ݒ�
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

	// Y���̒l�͈̔͐ݒ�
	diprg.diph.dwObj = DIJOFS_Y;
	hr = dev_gamepad[gamepad_no]->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(hr))
	{
		return hr;
	}

	// Z���̒l�͈̔͐ݒ�
	diprg.diph.dwObj = DIJOFS_Z;
	hr = dev_gamepad[gamepad_no]->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(hr))
	{
		return hr;
	}

	// RX���̒l�͈̔͐ݒ�
	diprg.diph.dwObj = DIJOFS_RX;
	hr = dev_gamepad[gamepad_no]->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(hr))
	{
		return hr;
	}

	// RY���̒l�͈̔͐ݒ�
	diprg.diph.dwObj = DIJOFS_RY;
	hr = dev_gamepad[gamepad_no]->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(hr))
	{
		return hr;
	}

	// RZ���̒l�͈̔͐ݒ�
	diprg.diph.dwObj = DIJOFS_RZ;
	hr = dev_gamepad[gamepad_no]->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(hr))
	{
		return hr;
	}

	return S_OK;
}
