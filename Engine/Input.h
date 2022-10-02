#pragma once

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#include "Property.h"
#include "Math/Vector2.h"


//===========================================
//
//		�T�u�N���X
//
//===========================================

// �R���g���[���[�̃{�^��
enum class GamePadButton
{
	INPUT_A,			//A�{�^��
	INPUT_B,			//B�{�^��
	INPUT_X,			//X�{�^��
	INPUT_Y,			//Y�{�^��
	INPUT_LB,			//LB�{�^��
	INPUT_RB,			//RB�{�^��
	INPUT_SELECT,		//SELECT�{�^��
	INPUT_START,		//START�{�^��
	INPUT_LSTICK,		//���X�e�B�b�N��������
	INPUT_RSTICK,		//�E�X�e�B�b�N��������
	INPUT_LEFT = 34,	//�\���L�[��
	INPUT_RIGHT = 32,	//�\���L�[�E
	INPUT_UP = 35,	//�\���L�[��
	INPUT_DOWN = 33,	//�\���L�[��
};
// �R���g���[���[�̃X�e�B�b�N
enum class GamePadStick
{
	Stick_L,
	Stick_R,
};

enum class GamePadAxis
{
	AXIS_LX = 0x01,			//���X�e�B�b�NX��
	AXIS_LY = 0x02,			//���X�e�B�b�NY��
	AXIS_LZ = 0x04,			//���X�e�B�b�NZ��(LT�ERT)
	AXIS_RX = 0x08,			//�E�X�e�B�b�NX��
	AXIS_RY = 0x10,			//�E�X�e�B�b�NY��
	AXIS_RZ = 0x20,			//�E�X�e�B�b�NZ��
};

// ���͂̍X�V��FixedUpdate�̃^�C�~���O�ōs����
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
	
	/// <summary>
	/// �{�^������͂��Ă���t���[������Ԃ�
	/// </summary>
	/// <returns>int ���̓t���[����</returns>
	static int GetButtonPress(GamePadButton button);

	/// <summary>
	/// �{�^���������ꂽ�u�Ԃ���true�ɂȂ�
	/// </summary>
	/// <returns>bool �����ꂽ�u��</returns>
	static bool GetButtonPressTrigger(GamePadButton button);

	/// <summary>
	/// �{�^���𗣂���Ă���t���[������Ԃ�
	/// </summary>
	/// <returns>int �����̓t���[����</returns>
	static int GetButtonReleas(GamePadButton button);

	/// <summary>
	/// �{�^���������ꂽ�u�Ԃ���true�ɂȂ�
	/// </summary>
	/// <returns>bool �����ꂽ�u��</returns>
	static bool GetButtonReleasTrigger(GamePadButton button);

	/// <summary>
	/// �X�e�B�b�N�̓��͏�Ԃ�Ԃ�
	/// </summary>
	/// <returns>Vector2 ���͊p�x</returns>
	static Vector2 GetStick(GamePadStick stick, const float &dead_zone = 0.2f, const size_t &gamepad_no = 0);

	/// <summary>
	/// �e���̓��͂�Ԃ�
	/// </summary>
	/// <returns>float ���̌X���x��(0.0~1.0)</returns>
	static float GetAxis(GamePadAxis axis,const float &dead_zone = 0.2f,const size_t& gamepad_no = 0);


private:

	Input(){};
	~Input(){};




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
	static void ControllerUpdate();
	static std::vector<IDirectInputDevice8 *> dev_gamepads_; //�Q�[���p�b�h�f�o�C�X
	static std::vector<DIJOYSTATE> gamepads_; //�Q�[���p�b�h�̐��̏��
	static int controller_press_count_[64];		// �R���g���[���[�������m�F�p
	static int controller_releas_count_[64];	// �R���g���[���[������m�F�p
	static const LONG zone_max_;					// �X�e�B�b�N���͂̏��
	static const LONG zone_min_;					// �X�e�B�b�N���͂̉���
};

