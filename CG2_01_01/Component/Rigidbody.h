#pragma once

#include "Vector3.h"
#include "Component/Component.h"

#include <DirectXMath.h>

enum class ForceMode
{
	Force,
	Impulse,
};

class Rigidbody :
	public Component
{
public:
	Rigidbody();

	void Infomation() override;


	void AddForce(XMFLOAT3 force, ForceMode foce_mode = ForceMode::Force);
	void AddForce(float x, float y, float z, ForceMode foce_mode = ForceMode::Force);
private:

	// ���W�b�h�{�f�B
	float mass_;         // ����
	float drag_;         // �͂œ����ꍇ�̋�C��R
	float angular_drag_; // �g���N�œ����ꍇ�̋�C��R
	bool use_gravity_;   // �d�͂��g�p���邩
	Vector3 velocity_;	// �����x�i����񐄏��j

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

