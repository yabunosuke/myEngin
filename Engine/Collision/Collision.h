#pragma once
#include "Object/GameObject/GameObject.h"
#include <memory>
#include "Property.h"
class Collision
{
public:
	Collision(
		std::weak_ptr<GameObject> game_object,
		Vector3 relative_velocity
		);

	// 衝突した相手のオブジェクト (ReadOnly)
	yEngin::Property<std::weak_ptr<GameObject>> gameObject
	{
		game_object_,
		yEngin::AccessorType::ReadOnly
	};
	// 衝突した2つのオブジェクトの相対的な速度 (ReadOnly)
	yEngin::Property<Vector3> relativeVelocity
	{
		relative_velocity_,
		yEngin::AccessorType::ReadOnly
	};


private:

	std::weak_ptr<GameObject> game_object_;
	Vector3 relative_velocity_;
};

