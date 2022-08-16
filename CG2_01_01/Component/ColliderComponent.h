#pragma once
#include "Component/Component.h"

#include "BaseCollider.h"

class AbstractScene;

class ColliderComponent final :
	public Component
{
public:
	ColliderComponent(AbstractScene *scene, CollisionShapeType type = SHAPE_SPHERE);
	~ColliderComponent();
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


private:
	// �R���C�_�[
	std::shared_ptr<BaseCollider> collider;
	CollisionShapeType type_;
};
