#include "OBBCollider.h"

OBBCollider::OBBCollider(Quaternion q, Vector3 extent, Vector3 center)
{
	local_extent_ = extent;
	local_center_ = center;
	local_quaternion_ = q;
	collision_type_ = CollisonType::OBB;
}

void OBBCollider::ComponentInitialize()
{
	game_object_->AddCollider(this);
}

void OBBCollider::ComponentUpdate()
{
	// 半径にスケール適応
	extent = Vector3::Scale(local_extent_ *0.5f, transform_->scale);
	// オフセット適応
	center = local_center_ + transform_->position;
	auto test = local_quaternion_ * Vector3::forward;

	// 回転適応
	Quaternion q = transform_->quaternion * local_quaternion_;
	unidirectional[0] = Vector3::Normalize(Vector3(q * Vector3::right));
	unidirectional[1] = Vector3::Normalize(Vector3(q * Vector3::up));
	unidirectional[2] = Vector3::Normalize(Vector3(q * Vector3::forward));

}

void OBBCollider::Infomation()
{
	ImGui::DragFloat3("Center", &local_center_.x);
	ImGui::DragFloat3("Extent", &local_extent_.x);


	if (ImGui::TreeNode("Info")) {
		// 操作不可、確認用
		ImGui::BeginDisabled();
		// 実際の中心
		ImGui::DragFloat3("World Center", &center.x);
		// 実際のスケール
		ImGui::DragFloat3("World Extent", &extent.x);
		// 各軸ベクトル
		ImGui::DragFloat3("Unidirectional Right", &unidirectional[0].x);
		ImGui::DragFloat3("Unidirectional Up", &unidirectional[1].x);
		ImGui::DragFloat3("Unidirectional Forward", &unidirectional[2].x);

		ImGui::EndDisabled();

		ImGui::TreePop();
	}

}
