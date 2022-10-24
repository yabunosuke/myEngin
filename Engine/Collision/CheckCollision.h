#pragma once
#include "Collision.h"

#include "Object/GameObject/GameObject.h"
#include "Object/Component/Collider/SphereCollider/SphereCollider.h"
#include "Object/Component/Collider/OBBCollider/OBBCollider.h"
#include "Primitive.h"

class CheckCollision final
{
public:

	static void CheckColliders(const std::vector<GameObject *> &game_objects);
	static bool CheckHit(Collider *a, Collider *b,Vector3 hit_pos);

	// AABB to AABB
	static bool AABB2AABB(yEngine::AABB a, yEngine::AABB b);
	static void ExtremePointsAlongDirection(
		const Vector3 &dir,
		Vector3 point[],
		int n,
		int *min, int *max
	);
	// Sphere to Sphere
	static bool Sphere2Sphere(yEngine::Sphere a, yEngine::Sphere b,Vector3 &hit_point);
	// 点に対するOBBの最近接点
	static void ClosestPtPoint2OBB(const Vector3 &point, const yEngine::OBB &obb, Vector3 &closest_point);
	// Sphere to OBB
	static bool Sphere2OBB(const yEngine::Sphere &sphere, const yEngine::OBB &obb, Vector3 &closest_point);
	
	// OBB to OBB
	static bool OBB2OBB(yEngine::OBB a, yEngine::OBB b);
	// Sphere to Capsule
	static bool Spphere2Capsule(yEngine::Sphere sphere, yEngine::Capsule capsule);

	// 線分と点の距離の平方を返却
	static float SqDistancePointSegment(Vector3 start,Vector3 end,Vector3 point);

private:
	static void OnTriggerEnter(GameObject *object, Collision &collision_data, const Vector3 &hit_pos);
	static void OnTriggerStay(GameObject *object, Collision &collision_data, const Vector3 &hit_pos);


	// 自分と親のOnCollisionを呼び出す
	static void OnCollisionEnter(GameObject *object,Collision &collision_data,const Vector3 &hit_pos);
	static void OnCollisionStay(GameObject *object,Collision &collision_data, const Vector3 &hit_pos);
};
