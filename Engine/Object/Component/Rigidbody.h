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

enum class RigidbodyInterpolation
{
	None,			// ��Ԃ��s��Ȃ�
	Interpolate,	// ��Ԃ��s��
	Extrapolate,	// �O�}��Ԃ��s��
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

	/// <summary>
	/// ���W�b�h�{�f�B�ɗ͂�������
	/// </summary>
	/// <param name="force">���[���h���W��ł̗͂̃x�N�g��</param>
	/// <param name="force_mode">�K������͂̃^�C�v</param>
	void AddForce(const Vector3 &force, ForceMode force_mode = ForceMode::Force);
	/// <summary>
	/// ���W�b�h�{�f�B�ɗ͂�������
	/// </summary>
	/// <param name="x, y, z">���[���h���W��ł̗͂̃x�N�g��</param>
	/// <param name="force_mode">�K������͂̃^�C�v</param>
	void AddForce(float x,float y,float z, ForceMode force_mode = ForceMode::Force);


	/// <summary>
	/// �I�u�W�F�N�g���X���[�v��ԉ���
	/// </summary>
	/// <returns></returns>
	bool IsSleep();


	yEngine::Property<Vector3> velocity
	{
		&velocity_,
		yEngine::AccessorType::AllAccess,
		nullptr,
		nullptr,
	
	};

	yEngine::Property<bool> useGravity
	{
		&use_gravity_,
		yEngine::AccessorType::AllAccess
	};


private:

	// ���W�b�h�{�f�B
	float angular_drag_ { 1.0f };			// �g���N�œ����ꍇ�̋�C��R

	float dinamic_friction_{ 0.6f };		// �����I�u�W�F�N�g�ɑ΂��門�C
	float static_friction_{ 0.6f };			// �Î~�����I�u�W�F�N�g�ɑ΂��門�C
	float bounciness_{ 0.0f };				// �����W��
	enum class FrictionCombine				// �Փ˂����I�u�W�F�N�g�Ԃ̖��C���ǂ��������邩
	{
		Average,	// ����
		Minimum,	// �����������g�p
		Maximum,	// �傫�������g�p
		Multiply,	// ��Z
	}friction_combine_ = FrictionCombine::Average;
	enum class BounceCombine				// �Փ˂����I�u�W�F�N�g�Ԃ̒��˕Ԃ����ǂ��������邩
	{
		Average,	// ����
		Minimum,	// �����������g�p
		Maximum,	// �傫�������g�p
		Multiply,	// ��Z
	}bounce_combine_ = BounceCombine::Average;

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

	// �I�u�W�F�N�g�̍R��(��C��R)
	float drag_{ 1.0f };			
	// �Œ�t���[�����[�g���Ɉړ���ۊǂ���
	RigidbodyInterpolation interpolation_;
	// ����
	float mass_{ 1.0f };
	// �I�u�W�F�N�g���X���[�v��Ԃɓ���臒l
	float sleep_threshold_{ 0.005f };
	// �d�͂̉e�����󂯂邩
	bool use_gravity_{ false };
	// ���x�x�N�g��
	Vector3 velocity_{ 0.0f,0.0f ,0.0f };		
	//���[���h���W��ł̎��ʂ̒��S�ʒu
	Vector3 world_center_of_mass_;





};

