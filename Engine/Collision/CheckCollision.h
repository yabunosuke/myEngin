#pragma once

#include "Object/GameObject/GameObject.h"
#include "Object/Component/Collider/SphereCollider/SphereCollider.h"
#include "Primitive.h"

class CheckCollision final
{
public:
	static void CheckColliders(const std::vector<GameObject *> &game_objects);
	static bool CheckHit(Collider *a, Collider *b,Vector3 hit_pos);

	// AABB to AABB
	static bool AABB2AABB(yEngine::AABB a, yEngine::AABB b);
	// Sphere to Sphere
	static bool Sphere2Sphere(yEngine::Sphere a, yEngine::Sphere b);
	// OBB to OBBi–¢À‘•j
	static bool OBB2OBB(yEngine::OBB a, yEngine::OBB b);
	// Sphere to Capsule
	static bool Spphere2Capsule(yEngine::Sphere sphere, yEngine::Capsule capsule);

	// ü•ª‚Æ“_‚Ì‹——£‚Ì•½•û‚ğ•Ô‹p
	static float SqDistancePointSegment(Vector3 start,Vector3 end,Vector3 point);

private:

};
