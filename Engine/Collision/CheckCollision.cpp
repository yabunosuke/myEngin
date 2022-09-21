#include "CheckCollision.h"
#include "Collision.h"
#include <memory>
#include "Object/Component/Rigidbody.h"
#include "Object/Component/Behaviour/MonoBehaviour/MonoBehaviour.h"
void CheckCollision::CheckColliders(const std::vector<std::weak_ptr<GameObject>> &game_objects)
{

	// コライダーのイテレーター
	std::vector<std::weak_ptr<Collider>>::const_iterator collider_it_a;
	std::vector<std::weak_ptr<Collider>>::const_iterator collider_it_b;

	// 全てのオブジェクトをチェック
	for (auto object_a = game_objects.begin(); object_a != game_objects.end(); ++object_a)
	{
		auto rigidbody_a = object_a->lock()->GetComponent<Rigidbody>();
		// Bオブジェクトのイテレータを一つずらす
		auto object_b = object_a;
		++object_b;

		for (; object_b != game_objects.end(); ++object_b)
		{
			auto rigidbody_b = object_b->lock()->GetComponent<Rigidbody>();

			// ブロードフェーズ(後ほど実装) 
			if (false) {
				// ブロードフェーズが衝突していなければ次のオブジェクトへ
				continue;
			}

			Vector3 hit_pos = {0,0,0};

			// ナローフェーズ
			for (std::weak_ptr<Collider> collider_a : object_a->lock().get()->GetColliders())
			{
				for (std::weak_ptr<Collider> collider_b : object_b->lock().get()->GetColliders())
				{

					// 当たり判定情報
					Collision collision_info_a{
						object_a->lock(),
						{ 0,20,0 }
					};
					Collision collision_info_b{
						object_b->lock(),
						{ 0,20,0 }
					};


					// 衝突判定
					if (CheckHit(collider_a, collider_b, hit_pos))
					{
						// Aの処理
						// 始めて衝突した場合と前回のアップデートで衝突していなかった場合Enter呼び出し
						if (
							collider_a.lock()->hitlist_[collider_b.lock()->GetInstanceID()] == false ||
							collider_a.lock()->hitlist_.count(collider_b.lock()->GetInstanceID()) == 0
							)
						{
							for (const auto &script_a : object_a->lock().get()->GetMonoBehaviours()) 
							{
								script_a.lock()->OnCollisionEnter(collision_info_a);
							}
							collider_a.lock()->hitlist_[collider_b.lock()->GetInstanceID()] = true;
						}
						// 前回のアップデートで衝突していた場合
						else
						{
							for (const auto &script_a : object_a->lock().get()->GetMonoBehaviours()) 
							{
								script_a.lock()->OnCollisionStay(collision_info_a);
							}

						}

					}
					else
					{
						// 接触が解除された場合
						if (
							collider_a.lock()->hitlist_.count(collider_b.lock()->GetInstanceID()) == 1 &&
							collider_a.lock()->hitlist_[collider_b.lock()->GetInstanceID()] == true
							)
						{
							for (const auto &script_a : object_a->lock().get()->GetMonoBehaviours()) {
								script_a.lock()->OnCollisionExit(collision_info_a);
							}
							collider_a.lock()->hitlist_[collider_b.lock()->GetInstanceID()] = false;

						}
					}
				}

			}

		}
	}
}

bool CheckCollision::CheckHit(std::weak_ptr<Collider> a, std::weak_ptr<Collider> b, Vector3 hit_pos)
{
	int collision_pattern = static_cast<int>(a.lock()->collisionType.r_) | static_cast<int>(b.lock()->collisionType.r_);

	if(collision_pattern == static_cast<int>(CollisonType::Sphere))
	{
		std::weak_ptr<SphereCollider> sphere_a = std::static_pointer_cast<SphereCollider>(a.lock());
		std::weak_ptr<SphereCollider> sphere_b = std::static_pointer_cast<SphereCollider>(b.lock());

		if (Sphere2Sphere(*sphere_a.lock().get(), *sphere_b.lock().get()))
		{
			return true;
		}
	}

	return false;
}

bool CheckCollision::AABB2AABB(yEngine::AABB a, yEngine::AABB b)
{
	if (fabsf(a.center[0] - b.center[0]) > (a.radius[0] + b.radius[0]))	return true;
	if (fabsf(a.center[1] - b.center[1]) > (a.radius[1] + b.radius[1]))	return true;
	if (fabsf(a.center[2] - b.center[2]) > (a.radius[2] + b.radius[2]))	return true;
	
	return false;
}

bool CheckCollision::Sphere2Sphere(yEngine::Sphere a, yEngine::Sphere b)
{
	Vector3 distance = a.center - b.center;
	float distance2 = Vector3::Dot(distance, distance);
	float radius_sum = a.radius + b.radius;

	return distance2 <= radius_sum * radius_sum;
}

bool CheckCollision::OBB2OBB(yEngine::OBB a, yEngine::OBB b) 
{

	return false;
}

bool CheckCollision::Spphere2Capsule(yEngine::Sphere sphere, yEngine::Capsule capsule)
{
	float distance2 = SqDistancePointSegment(capsule.start,capsule.end,sphere.center);
	float radius = sphere.radius + capsule.radius;

	return distance2 < radius * radius;
}

float CheckCollision::SqDistancePointSegment(Vector3 start, Vector3 end, Vector3 point)
{
	Vector3 start_to_end = end - start, start_to_point = point - start, end_to_point = point - end;
	float e = Vector3::Dot(start_to_point, start_to_end);

	// pointが線分の外側に射影される場合
	if (e <= 0.0f)
	{
		return Vector3::Dot(start_to_point, start_to_point);
	}
	float f = Vector3::Dot(start_to_end, start_to_end);
	if (e >= f)
	{
		return Vector3::Dot(end_to_point, end_to_point);
	}

	// pointが線分上に射影される場合
	return Vector3::Dot(start_to_point, start_to_point) - e * e / f;
}
