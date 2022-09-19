#include "CheckCollision.h"
#include "Collision.h"
#include <memory>
#include "Object/Component/Rigidbody.h"
#include "Object/Component/Behaviour/MonoBehaviour/MonoBehaviour.h"
void CheckCollision::CheckColliders(const std::vector<std::weak_ptr<GameObject>> &game_objects)
{

	// �R���C�_�[�̃C�e���[�^�[
	std::vector<std::weak_ptr<Collider>>::const_iterator collider_it_a;
	std::vector<std::weak_ptr<Collider>>::const_iterator collider_it_b;

	// �S�ẴI�u�W�F�N�g���`�F�b�N
	for (auto object_a = game_objects.begin(); object_a != game_objects.end(); ++object_a)
	{
		auto rigidbody_a = object_a->lock()->GetComponent<Rigidbody>();
		// B�I�u�W�F�N�g�̃C�e���[�^������炷
		auto object_b = object_a;
		++object_b;

		for (; object_b != game_objects.end(); ++object_b)
		{
			auto rigidbody_b = object_b->lock()->GetComponent<Rigidbody>();

			// �u���[�h�t�F�[�Y(��قǎ���) 
			if (false) {
				// �u���[�h�t�F�[�Y���Փ˂��Ă��Ȃ���Ύ��̃I�u�W�F�N�g��
				continue;
			}

			Vector3 hit_pos = {0,0,0};

			// �i���[�t�F�[�Y
			for (std::weak_ptr<Collider> collider_a : object_a->lock().get()->GetColliders())
			{
				for (std::weak_ptr<Collider> collider_b : object_b->lock().get()->GetColliders())
				{
					// �Փ˔���
					if (CheckHit(collider_a, collider_b, hit_pos))
					{
						// �����蔻����
						Collision collision_info_a{
							object_a->lock(),
							{ 0,20,0 }
						};
						Collision collision_info_b{
							object_a->lock(),
							{ 0,20,0 }
						};
						// �����ڐG

						// �Փ˃^�C�v�ʏ���
						if (true) {
							for (const auto &script_a : object_a->lock().get()->GetMonoBehaviours()) {
								script_a.lock()->OnCollisionEnter(collision_info_a);
							}
							for (const auto &script_b : object_b->lock().get()->GetMonoBehaviours()) {
								script_b.lock()->OnCollisionEnter(collision_info_b);
							}
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
