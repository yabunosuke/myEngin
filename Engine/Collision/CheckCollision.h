#pragma once
#include "Collision.h"

#include "Object/GameObject/GameObject.h"
#include "Object/Component/Rigidbody.h"
#include "Object/Component/Collider/SphereCollider/SphereCollider.h"
#include "Object/Component/Collider/OBBCollider/OBBCollider.h"
#include "Primitive.h"

class CheckCollision final
{
public:

	static void CheckColliders(const std::vector<GameObject *> &game_objects);
	static bool CheckHit(
		bool is_trigger, 
		Collider *a, 
		Collider *b,

		Vector3 &intrusion_a,
		Vector3 &intrusion_b,

		Collision &collision_a,
		Collision &collision_b
	);

	// AABB to AABB
	static bool AABB2AABB(yEngine::AABB a, yEngine::AABB b);
	static void ExtremePointsAlongDirection(
		const Vector3 &dir,
		Vector3 point[],
		int n,
		int *min, int *max
	);
	// Sphere to Sphere
	static bool Sphere2Sphere(
		bool is_trigger,
		const yEngine::Sphere &a,
		const yEngine::Sphere &b,

		Vector3 &intrusion_a,
		Vector3 &intrusion_b,

		Collision &collision_a,
		Collision &collision_b
	);
	// 点に対するOBBの最近接点
	static void ClosestPtPoint2OBB(const Vector3 &point, const yEngine::OBB &obb, Vector3 &closest_point);
	// Sphere to OBB
	static bool Sphere2OBB(
		bool is_trigger,
		const yEngine::Sphere &sphere,
		const yEngine::OBB &obb,

		Vector3 &intrusion_a,
		Vector3 &intrusion_b,

		Collision &collision_a,
		Collision &collision_b,

		Vector3 &closest_point
	
	);
	
	// OBB to OBB
	static bool OBB2OBB(yEngine::OBB a, yEngine::OBB b);
	// Sphere to Capsule
	static bool Spphere2Capsule(yEngine::Sphere sphere, yEngine::Capsule capsule);

	// 線分と点の距離の平方を返却
	static float SqDistancePointSegment(Vector3 start,Vector3 end,Vector3 point);

private:
	static void OnTriggerEnter(GameObject *object, Collider &other, const Vector3 &hit_pos);
	static void OnTriggerStay(GameObject *object, Collider &other, const Vector3 &hit_pos);

	// 自分と親のOnCollisionを呼び出す
	static void OnCollisionEnter(GameObject *object,Collision &collision_data,const Vector3 &hit_pos);
	static void OnCollisionStay(GameObject *object,Collision &collision_data, const Vector3 &hit_pos);

	// 衝突後の応答処理
	static void HitResponse(
		GameObject *object_a,
		Rigidbody *rigidbody_a,
		const Vector3 &intrusion_a,
		Collision &collision_data_a,

		GameObject *object_b,
		Rigidbody *rigidbody_b,
		const Vector3 &intrusion_b,
		Collision &collision_data_b
	);

};
