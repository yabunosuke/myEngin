#pragma once
#include "Math/Vector3.h"
#include "Object/Component/Component.h"
#include "Object/Component/Transform.h"

enum class ForceMode
{
	Force,			// �p���I�ɗ͂������� ���ʂ��l������
	Acceleration,	// �p���I�ɗ͂������� ���ʂ𖳎�����
	Impulse,		// �u�ԓI�ɗ͂������� ���ʂ��l������
	VelocityChange,	// �u�ԓI�ɗ͂������� ���ʂ𖳎�����
};

class Rigidbody :
	public Component
{
public:
	Rigidbody();

	void Infomation() override;

	void ComponentInitialize() override;
	void ComponentFixedUpdate() override;
	void ComponentUpdate() override;


	void AddForce(XMFLOAT3 force, ForceMode force_mode = ForceMode::Force);

	Vector3 velocity_{ 0.0f,0.0f ,0.0f };		// �����x�i����񐄏��j
private:

	// ���W�b�h�{�f�B
	float mass_			{ 1.0f };			// ����
	int drag_			{ 1 };			// �͂œ����ꍇ�̋�C��R�W��
	float angular_drag_ { 1.0f };			// �g���N�œ����ꍇ�̋�C��R
	bool use_gravity_	{ false };			// �d�͂��g�p���邩

	struct FreezePosition { // ���[���h���W�̎w�肵�����Ń��W�b�h�{�f�B�̈ړ����~
		bool x;
		bool y;
		bool z;
	}freeze_position_;

	struct FreezeRotation { // ���[�J�����W�̎w�肵�����Ń��W�b�h�{�f�B�̉�]���~
		bool x;
		bool y;
		bool z;
	}freeze_rotation_;


};

