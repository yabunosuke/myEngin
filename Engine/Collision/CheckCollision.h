#pragma once
#include "Collision.h"

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
	// OBB to OBB（未実装）
	static bool OBB2OBB(yEngine::OBB a, yEngine::OBB b);
	// Sphere to Capsule
	static bool Spphere2Capsule(yEngine::Sphere sphere, yEngine::Capsule capsule);

	// 線分と点の距離の平方を返却
	static float SqDistancePointSegment(Vector3 start,Vector3 end,Vector3 point);

private:
	// 自分と親のOnCollisionを呼び出す
	static void OnCollisionEnter(GameObject *object,Collision &collision_data);
	static void OnCollisionStay(GameObject *object,Collision &collision_data);
};
