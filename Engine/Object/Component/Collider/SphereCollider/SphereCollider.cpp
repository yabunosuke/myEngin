#include "SphereCollider.h"
#include "Object/GameObject/GameObject.h"

SphereCollider::SphereCollider(float radius, Vector3 center)
{
	local_radius_ = radius;
	local_center_ = center;
	collision_type_ = CollisonType::Sphere;
}

void SphereCollider::ComponentInitialize()
{
	// �I�u�W�F�N�g�ɃR���C�_�[waek_ptr�n��
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

	if (ImGui::TreeNode("Info")) {
		// ����s�A�m�F�p
		ImGui::BeginDisabled();
		// ���ۂ̒��S
		ImGui::DragFloat3("World Center", &center.x);
		// ���ۂ̃X�P�[��
		ImGui::DragFloat("World Extent", &radius);

		ImGui::EndDisabled();

		ImGui::TreePop();
	}
}
