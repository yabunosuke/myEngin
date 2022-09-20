#pragma once

#include "Object/GameObject/GameObject.h"
#include "Object/Component/Collider/SphereCollider/SphereCollider.h"
#include "Primitive.h"

class CheckCollision final
{
public:
	static void CheckColliders(const std::vector<std::weak_ptr<GameObject>> &game_objects);
	static bool CheckHit(std::weak_ptr<Collider> a, std::weak_ptr<Collider> b,Vector3 hit_pos);

	// AABB to AABB
	static bool AABB2AABB(yEngine::AABB a, yEngine::AABB b);
	// Sphere to Sphere
	static bool Sphere2Sphere(yEngine::Sphere a, yEngine::Sphere b);
	// OBB to OBB
	static bool OBB2OBB(yEngine::OBB a, yEngine::OBB b);


private:

};
