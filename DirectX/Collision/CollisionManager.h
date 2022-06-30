#pragma once

#include "CollisionPrimitive.h"
#include "RaycastHit.h"
#include "QueryCallback.h"

#include <d3d12.h>
#include <forward_list>

class BaseCollider;

class CollisionManager
{
public:// �ÓI�����o�֐�
	static CollisionManager *GetInstance();

public:// �����o�֐�
	// �R���C�_�[�̒ǉ�
	inline void AddCollider(BaseCollider *collider)
	{
		broadColliders.push_front(collider);
	}

	// �R���C�_�[�̍폜
	inline void RemoveBroadCollider(BaseCollider *collider)
	{
		broadColliders.remove(collider);
	}

	//�S�Ẵu���[�h�t�F�C�Y�Փ˃`�F�b�N
	void CheckBroadCollisions();

	//��̏Փ˃`�F�b�N
	bool CheckHitCollision(BaseCollider *colA, BaseCollider *colB, DirectX::XMVECTOR *inter = nullptr);

	// ���C�L���X�g
	//bool Raycast(const Ray& ray, RaycastHit* hitInfo = nullptr, float maxDistance = D3D12_FLOAT32_MAX);

	/*bool Raycast(const Ray& ray, unsigned short attribute, RaycastHit* hitInfo = nullptr,
		float maxDistance = D3D12_FLOAT32_MAX);*/

		// ���ɂ��ՓˑS����
	void QuerySphere(const Sphere &sphere, QueryCallback *callback);

private:
	CollisionManager() = default;
	CollisionManager(const CollisionManager &) = delete;
	~CollisionManager() = default;
	CollisionManager &operator=(const CollisionManager &) = delete;

	//�u���[�h�t�F�C�Y�p�̃R���C�_�[�̃��X�g
	std::forward_list<BaseCollider *> broadColliders;
};