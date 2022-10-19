#pragma once
#include "Object/GameObject/GameObject.h"
#include "Object/Component/Collider/Collider.h"
#include "ContactPoint.h"
#include <memory>
#include "Property.h"
class Collision
{
public:
	Collision(
		GameObject* game_object,
		Vector3 relative_velocity
		);

	// コライダーが衝突した相手のオブジェクト (ReadOnly)
	yEngine::Property<GameObject*> gameObject
	{
		game_object_,
		yEngine::AccessorType::ReadOnly
	};
	// 衝突した2つのオブジェクトの相対的な速度 (ReadOnly)
	yEngine::Property<Vector3> relativeVelocity
	{
		relative_velocity_,
		yEngine::AccessorType::ReadOnly
	};
	yEngine::Property<bool> childHit
	{
		is_child_hit_,
		yEngine::AccessorType::AllAccess
	};


private:
	
	Collider *collider_;	// ヒットしたコライダー
	std::vector<ContactPoint> contacts_;	
	GameObject *game_object_;
	Vector3 relative_velocity_;	
	bool is_child_hit_;
};

