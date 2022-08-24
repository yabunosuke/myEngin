#pragma once

#include "CollisionPrimitive.h"
#include "RaycastHit.h"
#include "QueryCallback.h"
#include "GameObjectManager.h"
#include "Object/GameObject/GameObject.h"

#include "Component/ColliderComponent.h"
#include <memory>

#include <d3d12.h>
#include <forward_list>

class BaseCollider;

class CollisionManager
{
public:
	struct BroadPhase {
		AABB broadphase_collider;
		std::vector<std::weak_ptr<ColliderComponent>> narrowphase_collider_list;
	};

public:// �����o�֐�
	// �R���C�_�[�̒ǉ�
	inline void AddCollider(std::shared_ptr<BaseCollider> collider)
	{
		colliders_.push_front(collider);
	}

	// �R���C�_�[�̍폜
	inline void RemoveBroadCollider(std::shared_ptr<BaseCollider> collider)
	{
		for (auto &col : colliders_) {
		}
	}

	//�S�Ẵu���[�h�t�F�C�Y�Փ˃`�F�b�N
	void CheckBroadCollisions(const std::vector<std::shared_ptr<GameObject>> &game_objects);

	//��̏Փ˃`�F�b�N
	bool CheckHitCollision(BaseCollider *colA, BaseCollider *colB, DirectX::XMVECTOR *inter = nullptr);

	// ���C�L���X�g
	//bool Raycast(const Ray& ray, RaycastHit* hitInfo = nullptr, float maxDistance = D3D12_FLOAT32_MAX);

	/*bool Raycast(const Ray& ray, unsigned short attribute, RaycastHit* hitInfo = nullptr,
		float maxDistance = D3D12_FLOAT32_MAX);*/

	// ���ɂ��ՓˑS����
	void QuerySphere(const Sphere &sphere, QueryCallback *callback);

private:

	// �S�ẴR���C�_�[
	std::forward_list<std::weak_ptr<BaseCollider>> colliders_;

};