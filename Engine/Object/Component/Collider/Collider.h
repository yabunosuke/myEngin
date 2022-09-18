#pragma once
#include "Object/Component/Component.h"
#include "BaseCollider.h"

class CollisionManager;

class AbstractScene;

class Collider :
	public Component
{
public:
	Collider(AbstractScene *scene, CollisionShapeType type = SHAPE_SPHERE);
	//~ColliderComponent();
	/// <summary>
	/// ������
	/// </summary>
	void ComponentInitialize() override;
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

private:
	// �R���C�_�[���g���K�[���ǂ���
	bool is_trigger_ = false;

	// �R���C�_�[
	std::shared_ptr<BaseCollider> collider;
	CollisionShapeType type_;
	std::shared_ptr<CollisionManager> collision_manager_;
};
