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

	// �Փ˂�������̃I�u�W�F�N�g (ReadOnly)
	yEngine::Property<std::weak_ptr<GameObject>> gameObject
	{
		game_object_,
		yEngine::AccessorType::ReadOnly
	};
	// �Փ˂���2�̃I�u�W�F�N�g�̑��ΓI�ȑ��x (ReadOnly)
	yEngine::Property<Vector3> relativeVelocity
	{
		relative_velocity_,
		yEngine::AccessorType::ReadOnly
	};


private:

	std::weak_ptr<GameObject> game_object_;
	Vector3 relative_velocity_;
};

