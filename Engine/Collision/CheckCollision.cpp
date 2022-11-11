#include "CheckCollision.h"
#include <memory>
#include "Object/Component/Behaviour/MonoBehaviour/MonoBehaviour.h"



void CheckCollision::CheckColliders(const std::vector<GameObject*> &game_objects)
{
	// �R���C�_�[�̃C�e���[�^�[
	std::vector<std::weak_ptr<Collider>>::const_iterator collider_it_a;
	std::vector<std::weak_ptr<Collider>>::const_iterator collider_it_b;
	
	// �S�ẴI�u�W�F�N�g���`�F�b�N
	for (auto object_a = game_objects.begin(); object_a != game_objects.end(); ++object_a)
	{
		// ��A�N�e�B�u�Ȃ�X�L�b�v
		if ((*object_a)->activeSelf == false) continue;

		// ��ԏ�̃I�u�W�F�N�g�̃��W�b�h�{�f�B
		auto rigidbody_top_a =
			static_cast<GameObject *>((*object_a)->top)->GetComponent<Rigidbody>();
		// A�I�u�W�F�N�g�̃��W�b�h�{�f�B
		auto rigidbody_a = (*object_a)->GetComponent<Rigidbody>();

		// B�I�u�W�F�N�g�̃C�e���[�^������炷
		auto object_b = object_a;
		++object_b;

		for (; object_b != game_objects.end(); ++object_b)
		{
			if ((*object_b)->activeSelf == false) continue;

			// ��ԏ�̃I�u�W�F�N�g�̃��W�b�h�{�f�B
			auto rigidbody_top_b =
				static_cast<GameObject *>((*object_b)->top)->GetComponent<Rigidbody>();
			// A�I�u�W�F�N�g�̃��W�b�h�{�f�B
			auto rigidbody_b = (*object_b)->GetComponent<Rigidbody>();

			// �����Ƃ����W�b�h�{�f�B���Ȃ��ꍇ�͏������s��Ȃ�
			if (rigidbody_top_a == nullptr &&
				rigidbody_top_b == nullptr)
			{
				continue;
			}


			// �u���[�h�t�F�[�Y(��قǎ���) 
			if (false) {
				// �u���[�h�t�F�[�Y���Փ˂��Ă��Ȃ���Ύ��̃I�u�W�F�N�g��
				continue;
			}

			Vector3 hit_pos = {0,0,0};

			// �����蔻����
			Collision collision_info_a;		// A�ɑ�����
			Collision collision_info_b;		// B�ɑ�����

			//Rigidbody *a_rigidbody = (*object_a)->GetComponent<Rigidbody>();
			//Rigidbody *b_rigidbody = (*object_b)->GetComponent<Rigidbody>();

			// �i���[�t�F�[�Y
			for (auto collider_a : (*object_a)->GetColliders())
			{
				for (auto collider_b : (*object_b)->GetColliders())
				{

					// �N���x
					Vector3 intrusion_a;		 // B�ɑ΂���A�̐N���x
					Vector3 intrusion_b;		 // A�ɑ΂���B�̐N���x

					// �g���K�[���ǂ���
					bool is_trigger = collider_a->isTrigger.r_ || collider_b->isTrigger.r_;

					// �Փ˔���
					if (CheckHit(
						is_trigger,
						collider_a,
						collider_b,

						intrusion_a,
						intrusion_b,

						collision_info_a,					
						collision_info_b				
					))
					{
						// �ڐG�����u��
						if (
							collider_b->hitlist_[collider_a->GetInstanceID()] == false ||
							collider_a->hitlist_.count(collider_b->GetInstanceID()) == 0
							)
						{
							// �g���K�[
							if(is_trigger)
							{
								OnTriggerEnter(*object_a, *collider_b, hit_pos);
								OnTriggerEnter(*object_b, *collider_a, hit_pos);
							}
							// �R���W����
							else
							{
								// �Փˉ���
								HitResponse(
									*object_a,
									rigidbody_a,
									intrusion_a,
									collision_info_a,

									*object_b,
									rigidbody_b,
									intrusion_b,
									collision_info_b
									);
								// �����߂�����
								OnCollisionEnter(*object_a, collision_info_a,hit_pos);
								OnCollisionEnter(*object_b, collision_info_b,hit_pos);

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
								OnTriggerStay(*object_a, *collider_b, hit_pos);
								OnTriggerStay(*object_b, *collider_a, hit_pos);
							}
							// �R���W����
							else
							{
								// �Փˉ���
								HitResponse(
									*object_a,
									rigidbody_a,
									intrusion_a,
									collision_info_a,

									*object_b,
									rigidbody_b,
									intrusion_b,
									collision_info_b
								);

								OnCollisionStay(*object_a, collision_info_a,hit_pos);
								OnCollisionStay(*object_b, collision_info_b,hit_pos);
							}
						}

						// �����߂�����

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
									script_a->OnTriggerExit(*collider_b);
								}

								// B
								for (const auto &script_b : (*object_b)->GetMonoBehaviours()) {
									script_b->OnTriggerExit(*collider_a);
								}
							}
							// �R���W����
							else
							{
								// A
								for (const auto &script_a : (*object_a)->GetMonoBehaviours()) {
									script_a->OnCollisionExit(collision_info_a);
								}

								// B
								for (const auto &script_b : (*object_b)->GetMonoBehaviours()) {
									script_b->OnCollisionExit(collision_info_b);
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

bool CheckCollision::CheckHit(
	bool is_trigger,
	Collider *a,
	Collider *b,

	Vector3 &intrusion_a,
	Vector3 &intrusion_b,

	Collision &collision_a,
	Collision &collision_b
)
{
	// �Փ˃^�C�v
	int collision_pattern = static_cast<int>(a->collisionType.r_) | static_cast<int>(b->collisionType.r_);

	bool is_hit{ false };
	// Sphere to Sphere
	if(collision_pattern == static_cast<int>(CollisonType::Sphere))
	{
		SphereCollider *sphere_a = static_cast<SphereCollider*>(a);
		SphereCollider *sphere_b = static_cast<SphereCollider*>(b);

		is_hit = Sphere2Sphere(
			is_trigger,
			*sphere_a, 
			*sphere_b,

			intrusion_a,
			intrusion_b,

			collision_a,
			collision_b

			
		);

		collision_a.game_object_ = sphere_b->game_object_;
		collision_b.game_object_ = sphere_a->game_object_;


		return is_hit;
	}

	if(collision_pattern == (static_cast<int>(CollisonType::Sphere) | static_cast<int>(CollisonType::OBB)))
	{
		SphereCollider *sphere;
		OBBCollider *obb;
		Vector3 closest_point;
		if((sphere = dynamic_cast<SphereCollider *>(a)) != nullptr )
		{
			obb = dynamic_cast<OBBCollider *>(b);
			collision_a.game_object_ = obb->game_object_;
			collision_b.game_object_ = sphere->game_object_;

			is_hit = Sphere2OBB(
				is_trigger,
				*sphere,
				*obb,
				intrusion_a,
				intrusion_b,

				collision_a,
				collision_b,

				closest_point);
		}
		else
		{
			sphere = static_cast<SphereCollider *>(b);
			obb = static_cast<OBBCollider *>(a);
			collision_a.game_object_ = sphere->game_object_;
			collision_b.game_object_ = obb->game_object_;


			is_hit = Sphere2OBB(
				is_trigger,
				*sphere,
				*obb,
				intrusion_b,
				intrusion_a,

				collision_b,
				collision_a,

				closest_point);

		}

		return is_hit;
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

bool CheckCollision::Sphere2Sphere(
	bool is_trigger,
	const yEngine::Sphere &a,
	const yEngine::Sphere &b,
	Vector3 &intrusion_a,
	Vector3 &intrusion_b,

	Collision &collision_a,
	Collision &collision_b
)
{
	Vector3 distance = a.center - b.center;
	float distance2 = Vector3::Dot(distance, distance);
	float radius_sum = a.radius + b.radius;

	bool is_hit = distance2 <= radius_sum * radius_sum;
	
	// �g���K�[�Ȃ珈���𒆒f
	if (is_trigger) return is_hit;

	// �G���^�[���Փ˂��Ă���ΏՓˉ����p�̌v�Z���s��
	if(is_hit)
	{

		// �ŋߐړ_
		Vector3 recently_contact_a		// A�I�u�W�F�N�g��B�ɍł��߂��_
		{
			(b.center - a.center).Normalized() * a.radius
		};
		if (!isfinite(recently_contact_a.Magnitude()))
		{
			recently_contact_a = Vector3::zero;
		}
		Vector3 recently_contact_b		// B�I�u�W�F�N�g��A�ɍł��߂��_
		{
			(a.center - b.center).Normalized() * b.radius
		};
		if (!isfinite(recently_contact_b.Magnitude()))
		{
			recently_contact_b = Vector3::zero;
		}
		collision_a.contactPoint->normal = (a.center - b.center).Normalized();
		collision_b.contactPoint->normal = (b.center - a.center).Normalized();


		// �N���x
		intrusion_a = (recently_contact_b - recently_contact_a);
		intrusion_b = (recently_contact_a - recently_contact_b);
		// �Փ˖@��
		collision_a.contact_point_.normal = (recently_contact_a - a.center).Normalized();
		collision_b.contact_point_.normal = (recently_contact_b - b.center).Normalized();

	}

	return is_hit;
}

void CheckCollision::ClosestPtPoint2OBB(const Vector3 &point, const yEngine::OBB &obb, Vector3 &closest_point,Vector3 &normal)
{
	Vector3 d = point - obb.center;

	closest_point = obb.center;
	float max{0.0f};
	// �e���ɑ΂���
	for(int i = 0;i < 3;++i)
	{
		// �{�b�N�X�̒��S���玲�ɉ����������𓾂�
		float dist = Vector3::Dot(d, obb.unidirectional[i]);
		// �{�b�N�X�͈̔͂����������傫���ꍇ�A�{�b�N�X�܂ŃN�����v
		if(dist >obb.extent[i])
		{
			if (max < dist)
			{
				max = dist;
				normal = obb.unidirectional[i];
			}
			dist = obb.extent[i];

		}


		if(dist < -obb.extent[i])
		{
			if (max < -dist)
			{
				max = dist;
				normal = -obb.unidirectional[i];
			}
			dist = -obb.extent[i];

		}
		closest_point += dist * obb.unidirectional[i];
	}
}

bool CheckCollision::Sphere2OBB(
	bool is_trigger,
	const yEngine::Sphere &sphere,
	const yEngine::OBB &obb,

	Vector3 &intrusion_sphere,
	Vector3 &intrusion_obb,

	Collision &collision_sphere,
	Collision &collision_obb,

	Vector3 &closest_point
)
{
	bool is_hit{false};
	Vector3 normal{ 0,0,0 };
	ClosestPtPoint2OBB(sphere.center, obb, closest_point,normal);
	
	Vector3 v = closest_point - sphere.center;

	is_hit = Vector3::Dot(v, v) <= sphere.radius * sphere.radius;

	// �g���K�[�Ȃ珈���𒆒f
	if (is_trigger) return is_hit;

	if (is_hit)
	{
		collision_sphere.contact_point_.point = closest_point;
		Vector3 closent_obb_2_radius{};
		float dot{ Vector3::Dot(closest_point - sphere.center,normal) };
		if(dot < 0.0f)
		{
			// �N���x

			//������Փ˓_
			closent_obb_2_radius =
				(closest_point - sphere.center).Normalized() * sphere.radius + sphere.center;
			intrusion_obb = closent_obb_2_radius - closest_point;
			intrusion_sphere = closest_point - closent_obb_2_radius;
			
		}
		else if(dot > 0.0f)
		{

			//������Փ˓_
			closent_obb_2_radius =
				( sphere.center - closest_point).Normalized() * sphere.radius + sphere.center;
			intrusion_obb = closent_obb_2_radius - closest_point;
			intrusion_sphere = closest_point - closent_obb_2_radius;

		}
		// ���S�ɏd�Ȃ��Ă���Ƃ�
		else
		{
			intrusion_obb = { 0,0,0 };
			intrusion_sphere = { 0,0,0 };
		}


		collision_sphere.contact_point_.normal = normal;
		collision_obb.contact_point_.normal = (closent_obb_2_radius - sphere.center).Normalized();


	}
	return is_hit;
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

void CheckCollision::OnTriggerEnter(GameObject *object, Collider &other, const Vector3 &hit_pos)
{
	// �����I�u�W�F�N�g�ɂ���X�N���v�g�Ăяo��
	for (const auto &script : object->GetMonoBehaviours())
	{
		script->OnTriggerEnter(other);
	}
}

void CheckCollision::OnTriggerStay(GameObject *object, Collider &other, const Vector3 &hit_pos)
{
	// �����I�u�W�F�N�g�ɂ���X�N���v�g�Ăяo��
	for (const auto &script : object->GetMonoBehaviours())
	{
		script->OnTriggerStay(other);
	}
}

void CheckCollision::OnCollisionEnter(GameObject *object, Collision &collision_data,const Vector3 &hit_pos)
{
	// �����I�u�W�F�N�g�ɂ���X�N���v�g�Ăяo��
	for (const auto &script_a : object->GetMonoBehaviours())
	{
		script_a->OnCollisionEnter(collision_data);
	}
}

void CheckCollision::OnCollisionStay(GameObject *object, Collision &collision_data, const Vector3 &hit_pos)
{
	// �����I�u�W�F�N�g�ɂ���X�N���v�g�Ăяo��
	for (const auto &script_a : object->GetMonoBehaviours())
	{
		script_a->OnCollisionStay(collision_data);
	}
}

void CheckCollision::HitResponse(
	GameObject *object_a,
	Rigidbody *rigidbody_a,
	const Vector3 &intrusion_a,
	Collision &collision_data_a,

	GameObject *object_b,
	Rigidbody *rigidbody_b,
	const Vector3 &intrusion_b,
	Collision &collision_data_b
	)
{
	
	// ��ԏ�̃I�u�W�F�N�g�̃g�����X�t�H�[����ω�������
	GameObject *top_parent_a{ object_a->top };
	GameObject *top_parent_b{ object_b->top };

	bool is_static_a
	{
		top_parent_a->isStatic ||
		(rigidbody_a == nullptr)
	};

	bool is_static_b
	{
		top_parent_b->isStatic ||	// �s���t���O�������Ă��邩
		(rigidbody_b == nullptr)
	};


	// �ǂ���������Ȃ��Ȃ牟���Ԃ��������s��Ȃ�
	if (is_static_a && is_static_b)
	{
		return;
	}

	Rigidbody *top_rigid_a = top_parent_a->GetComponent<Rigidbody>();
	Rigidbody *top_rigid_b = top_parent_b->GetComponent<Rigidbody>();

	// �����߂���
	Vector3 penalty_a;
	Vector3 penalty_b;

	// �ǂ���������ꍇ
	if (!is_static_a && !is_static_b)
	{

		// �߂荞�ݗʊm�F
		if (intrusion_a.Magnitude() <= Mathf::epsilon)
		{
			return;
		}
		if (Vector3::Dot(rigidbody_a->velocity->Normalized(), collision_data_a.contactPoint->normal) >= 0.0f)
		{
			return;
		}

		// ���ʂ̍��v
		float mass_total
		{
			(rigidbody_a ?
			rigidbody_a->mass_ :
			1.0f)
			+
			(rigidbody_b ?
			rigidbody_b->mass_ :
			1.0f)
		};

		float K{ 1.0f };
		float B{ 1.0f };
		
		// A
		Vector3 a_d{ intrusion_a };
		Vector3 a_relative_velocity
		{
			Vector3::Scale(rigidbody_a->velocity,collision_data_a.contactPoint->normal).Magnitude() * collision_data_a.contactPoint->normal
		};

		penalty_a = K * a_d + B * a_relative_velocity;
		rigidbody_a->velocity += penalty_a * rigidbody_a->mass_ / mass_total;

		//B
		Vector3 b_d{ intrusion_b };
		Vector3 b_relative_velocity
		{
			Vector3::Scale(rigidbody_b->velocity,collision_data_b.contactPoint->normal).Magnitude() * collision_data_b.contactPoint->normal
		};

		penalty_b = K * b_d + B * b_relative_velocity;
		rigidbody_b->velocity += penalty_b * rigidbody_a->mass_ / mass_total;
	}
	// a�������I�ȏꍇ
	else if(!is_static_a  && is_static_b)
	{

		if (Vector3::Dot(rigidbody_a->velocity->Normalized(), collision_data_a.contactPoint->normal) >= 0.0f)
		{
			return;
		}
		// �߂荞�݂����ȏ�Ȃ�y�i���e�B���ۂ�
		float K{ 1.0f };
		float B{ 1.0f };
		Vector3 d{ intrusion_a };
		Vector3 relative_velocity
		{
			Vector3::Scale(rigidbody_a->velocity,collision_data_a.contactPoint->normal).Magnitude() * collision_data_a.contactPoint->normal
		};

		penalty_a = K * d + B * relative_velocity;
		rigidbody_a->velocity += penalty_a;
	}
	// b�������I�ȏꍇ
	else if (is_static_a && !is_static_b)
	{
		
		if(Vector3::Dot(rigidbody_b->velocity->Normalized(), collision_data_b.contactPoint->normal)>=0.0f)
		{
			return;
		}
		// �߂荞�݂����ȏ�Ȃ�y�i���e�B���ۂ�
		float K{ 1.0f };
		float B{ 1.0f };
		Vector3 d{ intrusion_b };
		Vector3 relative_velocity
		{
			Vector3::Scale(rigidbody_b->velocity,collision_data_b.contactPoint->normal).Magnitude() * collision_data_b.contactPoint->normal
		};

		penalty_b = K * d + B * relative_velocity;
		rigidbody_b->velocity += penalty_b;

	}
}

