#include "KeyboardInput.h"
int KeyboardInput::pressCount[256] = { 0 };
int KeyboardInput::releasCount[256] = { 0 };
IDirectInputDevice8 *devkeyboard = nullptr;

KeyboardInput *KeyboardInput::GetIns()
{
    static KeyboardInput *instans;
	return instans;
}

void KeyboardInput::Initialize()
{
    IDirectInput8 *dinput = nullptr;
    HRESULT result;

    result = DirectInput8Create(
        WinApp::GetInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&dinput, nullptr);
    result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);
    result = devkeyboard->SetDataFormat(&c_dfDIKeyboard);   //•W€Œ`Ž®
    result = devkeyboard->SetCooperativeLevel(
        WinApp::GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
}

void KeyboardInput::Update()
{
    HRESULT result = devkeyboard->Acquire();
    BYTE key[256] = {};
    result = devkeyboard->GetDeviceState(sizeof(key), key);

    for (int i = 0; i < 256; i++) {
        if (key[i] != 0) {
            if (releasCount[i] != 0) {
                releasCount[i] = 0;
            }
            pressCount[i]++;
        }
        else {
            if (pressCount[i] != 0) {
                pressCount[i] = 0;
            }
            releasCount[i]++;
        }
    }
}

bool KeyboardInput::GetKeyPressT(int key)
{
    return pressCount[key] == 1;
}

bool KeyboardInput::GetKeyReleasT(int key)
{
    return releasCount[key] == 1;
}

int KeyboardInput::GetKeyPress(int key)
{
    return pressCount[key];
}

int KeyboardInput::GetKeyReleas(int key)
{
    return releasCount[key];
}
