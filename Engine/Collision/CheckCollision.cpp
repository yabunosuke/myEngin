#include "CheckCollision.h"
#include <memory>
#include "Object/Component/Rigidbody.h"
#include "Object/Component/Behaviour/MonoBehaviour/MonoBehaviour.h"
void CheckCollision::CheckColliders(const std::vector<GameObject*> &game_objects)
{

	// コライダーのイテレーター
	std::vector<std::weak_ptr<Collider>>::const_iterator collider_it_a;
	std::vector<std::weak_ptr<Collider>>::const_iterator collider_it_b;
	
	// 全てのオブジェクトをチェック
	for (auto object_a = game_objects.begin(); object_a != game_objects.end(); ++object_a)
	{
	
		auto rigidbody_a = (*object_a)->GetComponent<Rigidbody>();
		// Bオブジェクトのイテレータを一つずらす
		auto object_b = object_a;
		++object_b;

		for (; object_b != game_objects.end(); ++object_b)
		{
			auto rigidbody_b = (*object_b)->GetComponent<Rigidbody>();

			// ブロードフェーズ(後ほど実装) 
			if (false) {
				// ブロードフェーズが衝突していなければ次のオブジェクトへ
				continue;
			}

			Vector3 hit_pos = {0,0,0};

			// ナローフェーズ
			for (auto collider_a : (*object_a)->GetColliders())
			{
				for (auto collider_b : (*object_b)->GetColliders())
				{
					// 当たり判定情報
					Collision collision_info_a
					{
						*object_a,
						collider_b,
						{0,0,0}
					};
					Collision collision_info_b{
						*object_b,
						collider_a,
						{ 0,0,0 }
					};

					Rigidbody *a_rigidbody = (*object_a)->GetComponent<Rigidbody>();
					Rigidbody *b_rigidbody = (*object_b)->GetComponent<Rigidbody>();

					// 両方にRigidbodyがあれば速度の差を計算する
					if (
						a_rigidbody != nullptr &&
						b_rigidbody != nullptr
						)
					{
						collision_info_a.relative_velocity_ = a_rigidbody->velocity - b_rigidbody->velocity;
						collision_info_b.relative_velocity_ = b_rigidbody->velocity - a_rigidbody->velocity;
					}

					bool is_trigger = collider_a->isTrigger.r_ || collider_b->isTrigger.r_;

					// 衝突判定
					if (CheckHit(collider_a, collider_b, hit_pos))
					{
						// 接触した瞬間
						if (
							collider_a->hitlist_[collider_b->GetInstanceID()] == false ||
							collider_a->hitlist_.count(collider_b->GetInstanceID()) == 0
							)
						{
							// トリガー
							if(is_trigger)
							{
								OnTriggerEnter(*object_a, collision_info_b, hit_pos);
								OnTriggerEnter(*object_b, collision_info_a, hit_pos);
							}
							// コリジョン
							else
							{


								OnCollisionEnter(*object_a, collision_info_b,hit_pos);
								OnCollisionEnter(*object_b, collision_info_a,hit_pos);
							}
							// 衝突記憶
							collider_a->hitlist_[collider_b->GetInstanceID()] = true;
							collider_b->hitlist_[collider_a->GetInstanceID()] = true;
						}
						// 接触し続けている
						else
						{
							// トリガー
							if (is_trigger)
							{
								OnTriggerStay(*object_a, collision_info_b, hit_pos);
								OnTriggerStay(*object_b, collision_info_a, hit_pos);
							}
							// コリジョン
							else
							{
								OnCollisionStay(*object_a, collision_info_b,hit_pos);
								OnCollisionStay(*object_b, collision_info_a,hit_pos);
							}
						}

					}
					// 離脱判定
					else
					{
						// 接触解除
						if (
							collider_a->hitlist_.count(collider_b->GetInstanceID()) == 1 &&
							collider_a->hitlist_[collider_b->GetInstanceID()] == true
							)
						{
							// トリガー
							if (is_trigger)
							{
								// A
								for (const auto &script_a : (*object_a)->GetMonoBehaviours()) {
									script_a->OnTriggerExit(collision_info_b);
								}

								// B
								for (const auto &script_b : (*object_b)->GetMonoBehaviours()) {
									script_b->OnTriggerExit(collision_info_a);
								}
							}
							// コリジョン
							else
							{
								// A
								for (const auto &script_a : (*object_a)->GetMonoBehaviours()) {
									script_a->OnCollisionExit(collision_info_b);
								}

								// B
								for (const auto &script_b : (*object_b)->GetMonoBehaviours()) {
									script_b->OnCollisionExit(collision_info_a);
								}

							}
							// 離脱記憶
							collider_a->hitlist_[collider_b->GetInstanceID()] = false;
							collider_b->hitlist_[collider_a->GetInstanceID()] = false;

						}
					}
				}
			}
		}
	}
}

bool CheckCollision::CheckHit(Collider *a, Collider *b, Vector3 hit_pos)
{
	int collision_pattern = static_cast<int>(a->collisionType.r_) | static_cast<int>(b->collisionType.r_);

	// Sphere to Sphere
	if(collision_pattern == static_cast<int>(CollisonType::Sphere))
	{
		SphereCollider *sphere_a = static_cast<SphereCollider*>(a);
		SphereCollider *sphere_b = static_cast<SphereCollider*>(b);

		return Sphere2Sphere(*sphere_a, *sphere_b, hit_pos);
	}

	if(collision_pattern == (static_cast<int>(CollisonType::Sphere) | static_cast<int>(CollisonType::OBB)))
	{
		SphereCollider *sphere;
		OBBCollider *obb;
		if((sphere = dynamic_cast<SphereCollider *>(a)) != nullptr )
		{
			obb = dynamic_cast<OBBCollider *>(b);
		}
		else
		{
			sphere = static_cast<SphereCollider *>(b);
			obb = static_cast<OBBCollider *>(a);
		}
		Vector3 closest_point;

		return  Sphere2OBB(*sphere, *obb, closest_point);
	}
	if (collision_pattern == static_cast<int>(CollisonType::OBB))
	{
		OBBCollider *obb_a = static_cast<OBBCollider *>(a);
		OBBCollider *obb_b = static_cast<OBBCollider *>(b);

		return  OBB2OBB(*obb_a, *obb_b);
		
	}


	return false;
}

bool CheckCollision::AABB2AABB(yEngine::AABB a, yEngine::AABB b)
{
	if (fabsf(a.center[0] - b.center[0]) > (a.radius[0] + b.radius[0]))	return false;
	if (fabsf(a.center[1] - b.center[1]) > (a.radius[1] + b.radius[1]))	return false;
	if (fabsf(a.center[2] - b.center[2]) > (a.radius[2] + b.radius[2]))	return false;
	
	return true;
}

void CheckCollision::ExtremePointsAlongDirection(const Vector3 &dir, Vector3 point[], int n, int *min, int *max)
{

}

bool CheckCollision::Sphere2Sphere(yEngine::Sphere a, yEngine::Sphere b,Vector3 & hit_pos)
{
	Vector3 distance = a.center - b.center;
	float distance2 = Vector3::Dot(distance, distance);
	float radius_sum = a.radius + b.radius;

	bool is_hit = distance2 <= radius_sum * radius_sum;

	if(is_hit)
	{
		// 中心を衝突地点とする
		hit_pos = a.center + distance / 2.0f;
	}

	return is_hit;
}

void CheckCollision::ClosestPtPoint2OBB(const Vector3 &point, const yEngine::OBB &obb, Vector3 &closest_point)
{
	Vector3 d = point - obb.center;

	closest_point = obb.center;

	// 各軸に対して
	for(int i = 0;i < 3;++i)
	{
		// ボックスの中心から軸に沿った距離を得る
		float dist = Vector3::Dot(d, obb.unidirectional[i]);
		// ボックスの範囲よりも距離が大きい場合、ボックスまでクランプ
		if(dist >obb.extent[i])
		{
			dist = obb.extent[i];
		}
		if(dist < -obb.extent[i])
		{
			dist = -obb.extent[i];
		}

		closest_point += dist * obb.unidirectional[i];
	}
}

bool CheckCollision::Sphere2OBB(const yEngine::Sphere &sphere, const yEngine::OBB &obb, Vector3 &closest_point)
{
	ClosestPtPoint2OBB(sphere.center, obb, closest_point);

	Vector3 v = closest_point - sphere.center;


	return Vector3::Dot(v,v) <= sphere.radius * sphere.radius;
}

bool CheckCollision::OBB2OBB(yEngine::OBB a, yEngine::OBB b) 
{
	float radius_a, radius_b;
	XMFLOAT3X3 r, abs_r;

	// a座標内でbを表現する回転行列を計算
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++ j)
		{
			r.m[i][j] = Vector3::Dot(a.unidirectional[i], b.unidirectional[j]);
		}
	}
	// 平行行列ベクトルを計算
	Vector3 t{ b.center - a.center };
	// 平行移動をaの座標に変換
	t = Vector3{ Vector3::Dot(t,a.unidirectional[0]),Vector3::Dot(t,a.unidirectional[1]),Vector3::Dot(t,a.unidirectional[2]) };

	// 共通の部分式を計算
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			abs_r.m[i][j] = fabsf(r.m[i][j]) + Mathf::epsilon;
		}
	}

	// 軸 L = A0, L = A1, L = A2 を判定
	for (int i = 0; i < 3; ++i)
	{
		radius_a = a.extent[i];
		radius_b =
			b.extent[0] * abs_r.m[i][0] +
			b.extent[1] * abs_r.m[i][1] +
			b.extent[2] * abs_r.m[i][2];
		if(fabsf(t[i]) > radius_a + radius_b)
		{
			return  false;
		}
	}

	// 軸 L = B0, L = B1, L = B2 を判定
	for (int i = 0; i < 3; ++i)
	{
		radius_a =
			a.extent[0] * abs_r.m[0][i] +
			a.extent[1] * abs_r.m[1][i] +
			a.extent[2] * abs_r.m[2][i];
		radius_b = a.extent[i];
		if (fabsf(
			t[0] * r.m[0][i] +
			t[1] * r.m[1][i] +
			t[2] * r.m[2][i] ) >
			radius_a + radius_b)
		{
			return  false;
		}
	}

	// 軸 L = A0 x B0 を判定
	radius_a = a.extent[1] * abs_r.m[2][0] + a.extent[2] * abs_r.m[1][0];
	radius_b = b.extent[1] * abs_r.m[0][2] + b.extent[2] * abs_r.m[0][1];
	if(fabsf(
		t[2] *r.m[1][0] -
		t[1] *r.m[2][0]) >
		radius_a + radius_b)
	{
		return false;
	}

	// 軸 L = A0 x B1 を判定
	radius_a = a.extent[1] * abs_r.m[2][1] + a.extent[2] * abs_r.m[1][1];
	radius_b = b.extent[0] * abs_r.m[0][2] + b.extent[2] * abs_r.m[0][0];
	if (fabsf(
		t[2] * r.m[1][1] -
		t[1] * r.m[2][1]) >
		radius_a + radius_b)
	{
		return false;
	}

	// 軸 L = A0 x B2 を判定
	radius_a = a.extent[1] * abs_r.m[2][2] + a.extent[2] * abs_r.m[1][2];
	radius_b = b.extent[0] * abs_r.m[0][1] + b.extent[1] * abs_r.m[0][0];
	if (fabsf(
		t[2] * r.m[1][2] -
		t[1] * r.m[2][2]) >
		radius_a + radius_b)
	{
		return false;
	}

	// 軸 L = A1 x B1 を判定
	radius_a = a.extent[0] * abs_r.m[2][1] + a.extent[2] * abs_r.m[0][1];
	radius_b = b.extent[0] * abs_r.m[1][2] + b.extent[2] * abs_r.m[1][0];
	if (fabsf(
		t[0] * r.m[2][1] -
		t[2] * r.m[0][2]) >
		radius_a + radius_b)
	{
		return false;
	}

	// 軸 L = A1 x B2 を判定
	radius_a = a.extent[0] * abs_r.m[2][2] + a.extent[2] * abs_r.m[0][2];
	radius_b = b.extent[0] * abs_r.m[1][1] + b.extent[1] * abs_r.m[1][0];
	if (fabsf(
		t[0] * r.m[2][2] -
		t[2] * r.m[0][2]) >
		radius_a + radius_b)
	{
		return false;
	}

	// 軸 L = A2 x B0 を判定
	radius_a = a.extent[0] * abs_r.m[1][0] + a.extent[1] * abs_r.m[0][0];
	radius_b = b.extent[1] * abs_r.m[2][2] + b.extent[2] * abs_r.m[2][1];
	if (fabsf(
		t[1] * r.m[0][0] -
		t[0] * r.m[1][0]) >
		radius_a + radius_b)
	{
		return false;
	}

	// 軸 L = A2 x B1 を判定
	radius_a = a.extent[0] * abs_r.m[1][1] + a.extent[1] * abs_r.m[0][1];
	radius_b = b.extent[1] * abs_r.m[2][2] + b.extent[2] * abs_r.m[2][0];
	if (fabsf(
		t[1] * r.m[0][1] -
		t[0] * r.m[1][1]) >
		radius_a + radius_b)
	{
		return false;
	}

	// 軸 L = A2 x B2 を判定
	radius_a = a.extent[0] * abs_r.m[1][2] + a.extent[1] * abs_r.m[0][2];
	radius_b = b.extent[0] * abs_r.m[2][1] + b.extent[1] * abs_r.m[2][0];
	if (fabsf(
		t[1] * r.m[0][2] -
		t[0] * r.m[1][2]) >
		radius_a + radius_b)
	{
		return false;
	}

	return true;
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

void CheckCollision::OnTriggerEnter(GameObject *object, Collision &collision_data, const Vector3 &hit_pos)
{
	// 自分オブジェクトにあるスクリプト呼び出し
	for (const auto &script : object->GetMonoBehaviours())
	{
		script->OnTriggerEnter(collision_data);
	}

	// 親がいれば再起呼び出し
	if (object->GetPearent() != nullptr)
	{
		OnTriggerEnter(object->GetPearent(), collision_data, hit_pos);
	}
}

void CheckCollision::OnTriggerStay(GameObject *object, Collision &collision_data, const Vector3 &hit_pos)
{
	// 自分オブジェクトにあるスクリプト呼び出し
	for (const auto &script : object->GetMonoBehaviours())
	{
		script->OnTriggerStay(collision_data);
	}

	// 親がいれば再起呼び出し
	if (object->GetPearent() != nullptr)
	{
		OnTriggerStay(object->GetPearent(), collision_data, hit_pos);
	}
}

void CheckCollision::OnCollisionEnter(GameObject *object, Collision &collision_data,const Vector3 &hit_pos)
{
	// 自分オブジェクトにあるスクリプト呼び出し
	for (const auto &script_a : object->GetMonoBehaviours())
	{
		script_a->OnCollisionEnter(collision_data);
	}
	// 親がいれば再起呼び出し
	if (object->GetPearent() != nullptr)
	{
		OnCollisionEnter(object->GetPearent(), collision_data, hit_pos);
	}
}

void CheckCollision::OnCollisionStay(GameObject *object, Collision &collision_data, const Vector3 &hit_pos)
{
	// 自分オブジェクトにあるスクリプト呼び出し
	for (const auto &script_a : object->GetMonoBehaviours())
	{
		script_a->OnCollisionStay(collision_data);
	}
	collision_data.childHit = true;
	// 親がいれば再起呼び出し
	if (object->GetPearent() != nullptr)
	{
		OnCollisionStay(object->GetPearent(), collision_data, hit_pos);
	}
}
