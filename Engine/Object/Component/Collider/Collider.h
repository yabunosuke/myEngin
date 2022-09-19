#pragma once
#include "Object/Component/Component.h"
#include "BaseCollider.h"

enum class CollisonType
{
	None,
	Sphere,
	Box,
	Capsule,
	Mesh,

};

class Collider :
	public Component
{
public:
	Collider();
	
	/// <summary>
	/// �X�V
	/// </summary>
	void ComponentUpdate() override;
	/// <summary>
	/// �`��
	/// </summary>
	void ComponentDraw() override;
	/// <summary>
	/// �I������
	/// </summary>
	void ComponentFinalize() override;

	/// <summary>
	/// ���
	/// </summary>
	void Infomation() override;


	//===========================================
	//
	//	  �A�N�Z�b�T
	//
	//===========================================
	
	// �R���C�_�[���g���K�[���ǂ���
	yEngin::Property<bool> isTrigger
	{
		is_trigger_,
		yEngin::AccessorType::AllAccess
	};
	yEngin::Property<CollisonType> collisionType
	{
		collision_type_,
		yEngin::AccessorType::ReadOnly
	};
protected:
	// �R���C�_�[���g���K�[���ǂ���
	bool is_trigger_ = false;
	CollisonType collision_type_ = CollisonType::None;
};

