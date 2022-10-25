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

	Collision(
		GameObject* game_object,		// �Փ˂�������̃I�u�W�F�N�g
		Collider *collider,				// �Փ˂����R���C�_�[(����)
		Vector3 relative_velocity
		);

	// �R���C�_�[���Փ˂�������̃I�u�W�F�N�g (ReadOnly)
	yEngine::Property<GameObject*> gameObject
	{
		&game_object_,
		yEngine::AccessorType::ReadOnly
	};
	// �Փ˂����R���C�_�[ (ReadOnly)
	yEngine::Property<Collider *> collider
	{
		&collider_,
		yEngine::AccessorType::ReadOnly
	};
	// �Փ˂���2�̃I�u�W�F�N�g�̑��ΓI�ȑ��x (ReadOnly)
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
	Collider *collider_;		// �q�b�g�����R���C�_�[
	std::vector<ContactPoint> contacts_;	
	Vector3 relative_velocity_;	
	bool is_child_hit_;
};

