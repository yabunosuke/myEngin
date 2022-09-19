#pragma once

#include "Object/GameObject/GameObject.h"
#include "Object/Component/Collider/SphereCollider/SphereCollider.h"

class CheckCollision
{
public:
	static void CheckColliders(const std::vector<std::weak_ptr<GameObject>> &game_objects);
	static bool CheckHit(std::weak_ptr<Collider> a, std::weak_ptr<Collider> b,Vector3 hit_pos);

	static bool SphereToSphere(std::weak_ptr<SphereCollider> a, std::weak_ptr<SphereCollider> b);
private:

};
