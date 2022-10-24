#include "CheckCollision.h"
#include <memory>
#include "Object/Component/Rigidbody.h"
#include "Object/Component/Behaviour/MonoBehaviour/MonoBehaviour.h"
void CheckCollision::CheckColliders(const std::vector<GameObject*> &game_objects)
{

	// �R���C�_�[�̃C�e���[�^�[
	std::vector<std::weak_ptr<Collider>>::const_iterator collider_it_a;
	std::vector<std::weak_ptr<Collider>>::const_iterator collider_it_b;
	
	// �S�ẴI�u�W�F�N�g���`�F�b�N
	for (auto object_a = game_objects.begin(); object_a != game_objects.end(); ++object_a)
	{
	
		auto rigidbody_a = (*object_a)->GetComponent<Rigidbody>();
		// B�I�u�W�F�N�g�̃C�e���[�^������炷
		auto object_b = object_a;
		++object_b;

		for (; object_b != game_objects.end(); ++object_b)
		{
			auto rigidbody_b = (*object_b)->GetComponent<Rigidbody>();

			// �u���[�h�t�F�[�Y(��قǎ���) 
			if (false) {
				// �u���[�h�t�F�[�Y���Փ˂��Ă��Ȃ���Ύ��̃I�u�W�F�N�g��
				continue;
			}

			Vector3 hit_pos = {0,0,0};

			// �i���[�t�F�[�Y
			for (auto collider_a : (*object_a)->GetColliders())
			{
				for (auto collider_b : (*object_b)->GetColliders())
				{

					// �����蔻����
					Collision collision_info_a
					{
						*object_a,
						{ 0,20,0 }
					};
					Collision collision_info_b{
						*object_b,
						{ 0,20,0 }
					};

					bool is_trigger = collider_a->isTrigger.r_ || collider_b->isTrigger.r_;

					// �Փ˔���
					if (CheckHit(collider_a, collider_b, hit_pos))
					{
						// �ڐG�����u��
						if (
							collider_a->hitlist_[collider_b->GetInstanceID()] == false ||
							collider_a->hitlist_.count(collider_b->GetInstanceID()) == 0
							)
						{
							// �g���K�[
							if(is_trigger)
							{
								// A

								for (const auto &script_a : (*object_a)->GetMonoBehaviours())
								{
									script_a->OnTriggerEnter(collision_info_b);
								}

								// B
								for (const auto &script_b : (*object_b)->GetMonoBehaviours())
								{
									script_b->OnTriggerEnter(collision_info_a);
								}
							}
							// �R���W����
							else
							{

								OnCollisionEnter(*object_a, collision_info_b);
								OnCollisionEnter(*object_b, collision_info_a);
							}
							// �ՓˋL��
							collider_a->hitlist_[collider_b->GetInstanceID()] = true;
							collider_b->hitlist_[collider_a->GetInstanceID()] = true;
						}
						// �ڐG�������Ă���
						else
						{
							// �g���K�[
							if (is_trigger)
							{
								// A
								for (const auto &script_a : (*object_a)->GetMonoBehaviours())
								{
									script_a->OnTriggerStay(collision_info_b);
								}

								// B
								for (const auto &script_b : (*object_b)->GetMonoBehaviours())
								{
									script_b->OnTriggerStay(collision_info_a);
								}
							}
							// �R���W����
							else
							{

								OnCollisionStay(*object_a, collision_info_b);
								OnCollisionStay(*object_b, collision_info_a);
								
							}
						}

					}
					// ���E����
					else
					{
						// �ڐG����
						if (
							collider_a->hitlist_.count(collider_b->GetInstanceID()) == 1 &&
							collider_a->hitlist_[collider_b->GetInstanceID()] == true
							)
						{
							// �g���K�[
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
							// �R���W����
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
							// ���E�L��
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

	if(collision_pattern == static_cast<int>(CollisonType::Sphere))
	{
		SphereCollider *sphere_a = static_cast<SphereCollider*>(a);
		SphereCollider *sphere_b = static_cast<SphereCollider*>(b);

		if (Sphere2Sphere(*sphere_a, *sphere_b))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	if (collision_pattern == static_cast<int>(CollisonType::OBB))
	{
		OBBCollider *obb_a = static_cast<OBBCollider *>(a);
		OBBCollider *obb_b = static_cast<OBBCollider *>(b);

		if (OBB2OBB(*obb_a, *obb_b))
		{
			return true;
		}
		else
		{
			return false;
		}
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

bool CheckCollision::Sphere2Sphere(yEngine::Sphere a, yEngine::Sphere b)
{
	Vector3 distance = a.center - b.center;
	float distance2 = Vector3::Dot(distance, distance);
	float radius_sum = a.radius + b.radius;

	return distance2 <= radius_sum * radius_sum;
}

bool CheckCollision::OBB2OBB(yEngine::OBB a, yEngine::OBB b) 
{
	float radius_a, radius_b;
	XMFLOAT3X3 r, abs_r;

	// a���W����b��\�������]�s����v�Z
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++ j)
		{
			r.m[i][j] = Vector3::Dot(a.unidirectional[i], b.unidirectional[j]);
		}
	}
	// ���s�s��x�N�g�����v�Z
	Vector3 t{ b.center - a.center };
	// ���s�ړ���a�̍��W�ɕϊ�
	t = Vector3{ Vector3::Dot(t,a.unidirectional[0]),Vector3::Dot(t,a.unidirectional[1]),Vector3::Dot(t,a.unidirectional[2]) };

	// ���ʂ̕��������v�Z
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			abs_r.m[i][j] = fabsf(r.m[i][j]) + Mathf::epsilon;
		}
	}

	// �� L = A0, L = A1, L = A2 �𔻒�
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

	// �� L = B0, L = B1, L = B2 �𔻒�
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

	// �� L = A0 x B0 �𔻒�
	radius_a = a.extent[1] * abs_r.m[2][0] + a.extent[2] * abs_r.m[1][0];
	radius_b = b.extent[1] * abs_r.m[0][2] + b.extent[2] * abs_r.m[0][1];
	if(fabsf(
		t[2] *r.m[1][0] -
		t[1] *r.m[2][0]) >
		radius_a + radius_b)
	{
		return false;
	}

	// �� L = A0 x B1 �𔻒�
	radius_a = a.extent[1] * abs_r.m[2][1] + a.extent[2] * abs_r.m[1][1];
	radius_b = b.extent[0] * abs_r.m[0][2] + b.extent[2] * abs_r.m[0][0];
	if (fabsf(
		t[2] * r.m[1][1] -
		t[1] * r.m[2][1]) >
		radius_a + radius_b)
	{
		return false;
	}

	// �� L = A0 x B2 �𔻒�
	radius_a = a.extent[1] * abs_r.m[2][2] + a.extent[2] * abs_r.m[1][2];
	radius_b = b.extent[0] * abs_r.m[0][1] + b.extent[1] * abs_r.m[0][0];
	if (fabsf(
		t[2] * r.m[1][2] -
		t[1] * r.m[2][2]) >
		radius_a + radius_b)
	{
		return false;
	}

	// �� L = A1 x B1 �𔻒�
	radius_a = a.extent[0] * abs_r.m[2][1] + a.extent[2] * abs_r.m[0][1];
	radius_b = b.extent[0] * abs_r.m[1][2] + b.extent[2] * abs_r.m[1][0];
	if (fabsf(
		t[0] * r.m[2][1] -
		t[2] * r.m[0][2]) >
		radius_a + radius_b)
	{
		return false;
	}

	// �� L = A1 x B2 �𔻒�
	radius_a = a.extent[0] * abs_r.m[2][2] + a.extent[2] * abs_r.m[0][2];
	radius_b = b.extent[0] * abs_r.m[1][1] + b.extent[1] * abs_r.m[1][0];
	if (fabsf(
		t[0] * r.m[2][2] -
		t[2] * r.m[0][2]) >
		radius_a + radius_b)
	{
		return false;
	}

	// �� L = A2 x B0 �𔻒�
	radius_a = a.extent[0] * abs_r.m[1][0] + a.extent[1] * abs_r.m[0][0];
	radius_b = b.extent[1] * abs_r.m[2][2] + b.extent[2] * abs_r.m[2][1];
	if (fabsf(
		t[1] * r.m[0][0] -
		t[0] * r.m[1][0]) >
		radius_a + radius_b)
	{
		return false;
	}

	// �� L = A2 x B1 �𔻒�
	radius_a = a.extent[0] * abs_r.m[1][1] + a.extent[1] * abs_r.m[0][1];
	radius_b = b.extent[1] * abs_r.m[2][2] + b.extent[2] * abs_r.m[2][0];
	if (fabsf(
		t[1] * r.m[0][1] -
		t[0] * r.m[1][1]) >
		radius_a + radius_b)
	{
		return false;
	}

	// �� L = A2 x B2 �𔻒�
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

	// point�������̊O���Ɏˉe�����ꍇ
	if (e <= 0.0f)
	{
		return Vector3::Dot(start_to_point, start_to_point);
	}
	float f = Vector3::Dot(start_to_end, start_to_end);
	if (e >= f)
	{
		return Vector3::Dot(end_to_point, end_to_point);
	}

	// point��������Ɏˉe�����ꍇ
	return Vector3::Dot(start_to_point, start_to_point) - e * e / f;
}

void CheckCollision::OnCollisionEnter(GameObject *object, Collision &collision_data)
{
	// �����I�u�W�F�N�g�ɂ���X�N���v�g�Ăяo��
	for (const auto &script_a : object->GetMonoBehaviours())
	{
		script_a->OnCollisionEnter(collision_data);
	}
	// �e������΍ċN�Ăяo��
	if (object->GetPearent() != nullptr)
	{
		OnCollisionEnter(object->GetPearent(), collision_data);
	}
}

void CheckCollision::OnCollisionStay(GameObject *object, Collision &collision_data)
{
	// �����I�u�W�F�N�g�ɂ���X�N���v�g�Ăяo��
	for (const auto &script_a : object->GetMonoBehaviours())
	{
		script_a->OnCollisionStay(collision_data);
	}
	collision_data.childHit = true;
	// �e������΍ċN�Ăяo��
	if (object->GetPearent() != nullptr)
	{
		OnCollisionStay(object->GetPearent(), collision_data);
	}
}
