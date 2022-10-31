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
	static bool CheckHit(Collider *a, Collider *b,ContactPoint &contact_a, ContactPoint &contact_b);

	// AABB to AABB
	static bool AABB2AABB(yEngine::AABB a, yEngine::AABB b);
	static void ExtremePointsAlongDirection(
		const Vector3 &dir,
		Vector3 point[],
		int n,
		int *min, int *max
	);
	// Sphere to Sphere
	static bool Sphere2Sphere(const yEngine::Sphere &a, const yEngine::Sphere &b, ContactPoint &contact_a, ContactPoint &contact_b);
	// �_�ɑ΂���OBB�̍ŋߐړ_
	static void ClosestPtPoint2OBB(const Vector3 &point, const yEngine::OBB &obb, Vector3 &closest_point);
	// Sphere to OBB
	static bool Sphere2OBB(const yEngine::Sphere &sphere, const yEngine::OBB &obb, Vector3 &closest_point);
	
	// OBB to OBB
	static bool OBB2OBB(yEngine::OBB a, yEngine::OBB b);
	// Sphere to Capsule
	static bool Spphere2Capsule(yEngine::Sphere sphere, yEngine::Capsule capsule);

	// �����Ɠ_�̋����̕�����ԋp
	static float SqDistancePointSegment(Vector3 start,Vector3 end,Vector3 point);

private:
	static void OnTriggerEnter(GameObject *object, Collision &collision_data, const Vector3 &hit_pos);
	static void OnTriggerStay(GameObject *object, Collision &collision_data, const Vector3 &hit_pos);

	// �����Ɛe��OnCollision���Ăяo��
	static void OnCollisionEnter(GameObject *object,Collision &collision_data,const Vector3 &hit_pos);
	static void OnCollisionStay(GameObject *object,Collision &collision_data, const Vector3 &hit_pos);

	// �Փˌ�̉�������
	static void HitResponse(
		GameObject *object_a,
		Rigidbody *rigidbody_a,
		const ContactPoint &contact_a,

		GameObject *object_b,
		Rigidbody *rigidbody_b,
		const ContactPoint &contact_b
	);
};
