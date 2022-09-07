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
	//		�ÓI�ϐ�
	//
	//===========================================

	// �L�[�{�[�h

	//===========================================
	//
	//		�ÓI�֐�
	//
	//===========================================

	static void Initialize();		// ������
	static void Update();			// �X�V




	// �L�[�{�[�h

	/// <summary>
	/// �L�[����͂��Ă���t���[������Ԃ�
	/// </summary>
	/// <returns>int ���̓t���[����</returns>
	static int GetKeyPress(int key);

	/// <summary>
	/// �L�[�������ꂽ�u�Ԃ���true�ɂȂ�
	/// </summary>
	/// <returns>bool �����ꂽ�u��</returns>
	static bool GetKeyPressTrigger(int key);

	/// <summary>
	/// �L�[�𗣂��Ă���t���[������Ԃ�
	/// </summary>
	/// <returns>int ���̓t���[����</returns>
	static int GetKeyReleas(int key);

	/// <summary>
	/// �L�[�������ꂽ�u�Ԃ���true�ɂȂ�
	/// </summary>
	/// <returns>bool �����ꂽ�u��</returns>
	static bool GetKeyReleasTrigger(int key);

	// �R���g���[���[




private:

	Input(){};
	~Input(){};



	// �R���g���[���[

	// �f�o�C�X�񋓃R�[���o�b�N�֐�
	static BOOL CALLBACK EnumGamePadCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);
	// �v���p�e�B�Z�b�g
	static HRESULT SetUpGamePadProperty(const size_t &gamepad_no = 0);

	// ����
	static bool any_key_;			// �����ꂩ�̃L�[���}�E�X��������Ă����true

	// �L�[�{�[�h
	static int key_press_count_[256];	// �L�[�������m�F�p
	static int key_releas_count_[256];	// �L�[������m�F�p

	// �R���g���[���[
	static int controller_press_count_[64];		// �R���g���[���[�������m�F�p
	static int controller_releas_count_[64];	// �R���g���[���[������m�F�p
	static const LONG zone_max;					// �X�e�B�b�N���͂̏��
	static const LONG zone_min;					// �X�e�B�b�N���͂̉���
};

