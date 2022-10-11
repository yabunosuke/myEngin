#pragma once
#include "Object/GameObject/GameObject.h"
#include <memory>
#include "Property.h"
class Collision
{
public:
	Collision(
		GameObject* game_object,
		Vector3 relative_velocity
		);

	// 衝突した相手のオブジェクト (ReadOnly)
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


private:
	GameObject* game_object_;
	Vector3 relative_velocity_;
};

