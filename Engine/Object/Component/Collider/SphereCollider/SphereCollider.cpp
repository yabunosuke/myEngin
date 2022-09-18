#include "SphereCollider.h"

SphereCollider::SphereCollider(Vector3 center, float radius):
center_(center),
radius_(radius)
{
	collision_type_ = CollisonType::Sphere;
}

void SphereCollider::ComponentInitialize()
{
	// オブジェクトにコライダーwaek_ptr渡す
	game_object_.lock()->AddCollider(std::static_pointer_cast<Collider>(shared_from_this()));
}

void SphereCollider::Infomation()
{
	ImGui::Checkbox("IsTrigger", &is_trigger_);
	ImGui::DragFloat3("Center", reinterpret_cast<float*>(&center_));
	ImGui::DragFloat("Radius", &radius_);
}
