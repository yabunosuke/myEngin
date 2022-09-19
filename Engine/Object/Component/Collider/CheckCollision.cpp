#include "CheckCollision.h"

#include "Collider.h"
#include "SphereCollider/SphereCollider.h"
#include <memory>

void CheckCollision::CheckColliders(const std::vector<std::weak_ptr<GameObject>> &game_objects)
{

	// コライダーのイテレーター
	std::vector<std::weak_ptr<Collider>>::const_iterator collider_it_a;
	std::vector<std::weak_ptr<Collider>>::const_iterator collider_it_b;

	// 全てのオブジェクトをチェック
	for (auto object_a = game_objects.begin(); object_a != game_objects.end(); ++object_a)
	{
		// Bオブジェクトのイテレータを一つずらす
		auto object_b = object_a;
		++object_b;

		for (; object_b != game_objects.end(); ++object_b)
		{

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
					// 衝突判定
					if (CheckHitCollision(collider_a, collider_b, hit_pos))
					{
						// トリガー

						// 物理接触

						// 衝突タイプ別処理
						if (true) {
							for (const auto &script_a : object_a->lock().get()->GetScripts()) {
								std::static_pointer_cast<ScriptComponent>(script_a.lock())->OnCollisionEnter();
							}
							for (const auto &script_b : object_b->lock().get()->GetScripts()) {
								std::static_pointer_cast<ScriptComponent>(script_b.lock())->OnCollisionEnter();
							}
						}
					}
				}

			}

		}
	}
}

bool CheckCollision::CheckHitCollision(std::weak_ptr<Collider> a, std::weak_ptr<Collider> b, Vector3 hit_pos)
{
	int collision_pattern = static_cast<int>(a.lock()->collisionType.r_) | static_cast<int>(b.lock()->collisionType.r_);

	if(collision_pattern == static_cast<int>(CollisonType::Sphere))
	{
		std::weak_ptr<SphereCollider> sphere_a = std::static_pointer_cast<SphereCollider>(a.lock());
		std::weak_ptr<SphereCollider> sphere_b = std::static_pointer_cast<SphereCollider>(b.lock());
		if(SphereToSphere(sphere_a, sphere_b))
		{
			return true;
		}
	}

	return false;
}

bool CheckCollision::SphereToSphere(std::weak_ptr<SphereCollider> a, std::weak_ptr<SphereCollider> b)
{
	float distance =
		Vector3::Distance(
			(a.lock()->center + a.lock()->transform_.lock()->position),
			(b.lock()->center + b.lock()->transform_.lock()->position)
		);
	float radius =
		a.lock()->radius + b.lock()->radius;

	if(distance<= radius)
	{
		return  true;
	}
	return false;
}
