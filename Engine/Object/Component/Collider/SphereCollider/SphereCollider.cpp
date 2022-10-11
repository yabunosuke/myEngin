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
	game_object_->AddCollider(this);
}

void SphereCollider::ComponentUpdate()
{
	radius = local_radius_ * Mathf::Max(
		transform_->scale->x,
		transform_->scale->y,
		transform_->scale->z
	);
	center = local_center_ + transform_->position;
}

void SphereCollider::Infomation()
{
	ImGui::Checkbox("IsTrigger", &is_trigger_);
	ImGui::DragFloat3("Center", reinterpret_cast<float*>(&local_center_.x));
	ImGui::DragFloat("Radius", &local_radius_);
}
