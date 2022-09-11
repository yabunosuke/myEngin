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
public:// メンバ関数

	//全てのブロードフェイズ衝突チェック
	void CheckBroadCollisions(const std::vector<std::shared_ptr<GameObject>> &game_objects);

	//二つの衝突チェック
	bool CheckHitCollision(BaseCollider *colA, BaseCollider *colB, DirectX::XMVECTOR *inter = nullptr);

	// レイキャスト
	//bool Raycast(const Ray& ray, RaycastHit* hitInfo = nullptr, float maxDistance = D3D12_FLOAT32_MAX);

	/*bool Raycast(const Ray& ray, unsigned short attribute, RaycastHit* hitInfo = nullptr,
		float maxDistance = D3D12_FLOAT32_MAX);*/

	// 球による衝突全検索
	void QuerySphere(const Sphere &sphere, QueryCallback *callback);

};