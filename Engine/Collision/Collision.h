#pragma once
#include "Object/GameObject/GameObject.h"
#include "Object/Component/Collider/Collider.h"
#include "ContactPoint.h"
#include <memory>

#include "Property.h"

class Collision
{
public:
	friend class CheckCollision;


	// 衝突したオブジェクト情報 (ReadOnly)
	yEngine::Property<GameObject*> gameObject
	{
		&game_object_,
		yEngine::AccessorType::ReadOnly
	};
	// 衝突したコライダー情報 (ReadOnly)
	yEngine::Property<Collider *> collider
	{
		&collider_,
		yEngine::AccessorType::ReadOnly
	};
	// 衝突地点の情報
	yEngine::Property<ContactPoint> contactPoint
	{
		&contact_point_,
		yEngine::AccessorType::ReadOnly
	};
	
	// 衝突した2つのオブジェクトの相対的な速度 (ReadOnly)
	yEngine::Property<Vector3> relativeVelocity
	{
		&relative_velocity_,
		yEngine::AccessorType::ReadOnly
	};
	yEngine::Property<bool> childHit
	{
		&is_child_hit_,
		yEngine::AccessorType::AllAccess
	};


private:
	GameObject *game_object_;	
	Collider *collider_;		// ヒットしたコライダー
	ContactPoint contact_point_;	
	Vector3 relative_velocity_;	
	bool is_child_hit_;
};

