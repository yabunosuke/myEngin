#include "SphereCollider.h"

SphereCollider::SphereCollider(float radius, Vector3 center):
center_(center),
radius_(radius)
{
	collision_type_ = CollisonType::Sphere;
}

void SphereCollider::ComponentInitialize()
{
	// �I�u�W�F�N�g�ɃR���C�_�[waek_ptr�n��
	game_object_.lock()->AddCollider(std::static_pointer_cast<Collider>(shared_from_this()));
}

void SphereCollider::Infomation()
{
	ImGui::Checkbox("IsTrigger", &is_trigger_);
	ImGui::DragFloat3("Center", reinterpret_cast<float*>(&center_));
	ImGui::DragFloat("Radius", &radius_);
}
