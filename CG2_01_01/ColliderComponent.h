#pragma once
#include "Component.h"
#include "BaseCollider.h"

class AbstractScene;

class ColliderComponent :
	public Component
{
public:
	ColliderComponent(CollisionShapeType type, AbstractScene &scene);

	/// <summary>
	/// ������
	/// </summary>
	void VirtualInitialize() override;
	/// <summary>
	/// �X�V
	/// </summary>
	void VirtualUpdate() override;
	/// <summary>
	/// �`��
	/// </summary>
	void VirtualDraw() override;
	/// <summary>
	/// �I������
	/// </summary>
	void VirtualFinalize() override;

	/// <summary>
	/// ���
	/// </summary>
	void Infomation() override;


private:
	// �R���C�_�[
	std::shared_ptr<BaseCollider> collider;

};

