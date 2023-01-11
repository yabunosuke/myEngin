#include "SphereCollider.h"
#include "Object/GameObject/GameObject.h"

SphereCollider::SphereCollider(float radius, Vector3 center)
{
	local_radius_ = radius;
	local_center_ = center;
	collision_type_ = CollisonType::Sphere;
}

void SphereCollider::ComponentUpdate()
{
	radius = local_radius_ / 2.0f * Mathf::Max(
		transform->lock()->scale->x,
		transform->lock()->scale->y,
		transform->lock()->scale->z
	);
	center = local_center_ + transform->lock()->position;
}

void SphereCollider::Infomation()
{
	ImGui::Checkbox("IsTrigger", &is_trigger_);
	ImGui::DragFloat3("Center", reinterpret_cast<float*>(&local_center_.x));
	ImGui::DragFloat("Radius", &local_radius_);

	if (ImGui::TreeNode("Info")) {
		// 操作不可、確認用
		ImGui::BeginDisabled();
		// 実際の中心
		ImGui::DragFloat3("World Center", &center.x);
		// 実際のスケール
		ImGui::DragFloat("World Extent", &radius);

		ImGui::EndDisabled();

		ImGui::TreePop();
	}
}
