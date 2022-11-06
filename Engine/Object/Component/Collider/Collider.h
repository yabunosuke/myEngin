#pragma once
#include "Object/Component/Component.h"
#include "Math/Mathf.h"
#include "Math/Quaternion.h"
#include <map>

enum class CollisonType
{
	None	= -1,
	AABB	= 1 << 0,
	Sphere	= 1 << 1,
	OBB		= 1 << 2,
	Capsule = 1 << 3,

};

class Collider :
	public Component
{
public:
	Collider();
	~Collider();
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
	yEngine::Property<bool> isTrigger
	{
		&is_trigger_,
		yEngine::AccessorType::AllAccess
	};
	yEngine::Property<CollisonType> collisionType
	{
		&collision_type_,
		yEngine::AccessorType::ReadOnly
	};


	std::map<int, bool> hitlist_;
protected:
	// �R���C�_�[���g���K�[���ǂ���
	bool is_trigger_{ false };
	CollisonType collision_type_ = CollisonType::None;
};

