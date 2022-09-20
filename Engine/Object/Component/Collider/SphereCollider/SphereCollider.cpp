#include "SphereCollider.h"

SphereCollider::SphereCollider(float radius, Vector3 center)
{
	local_radius_ = radius;
	local_center_ = center;
	collision_type_ = CollisonType::Sphere;
}

void SphereCollider::ComponentInitialize()
{
	// オブジェクトにコライダーwaek_ptr渡す
	game_object_.lock()->AddCollider(std::static_pointer_cast<Collider>(shared_from_this()));
}

void SphereCollider::ComponentUpdate()
{
	radius = local_radius_ * Mathf::Max(
		transform_.lock()->scale->x,
		transform_.lock()->scale->y,
		transform_.lock()->scale->z
	);
	center = local_center_ + transform_.lock()->position;
}

void SphereCollider::Infomation()
{
	ImGui::Checkbox("IsTrigger", &is_trigger_);
	ImGui::DragFloat3("Center", reinterpret_cast<float*>(&local_center_.x));
	ImGui::DragFloat("Radius", &local_radius_);
}
