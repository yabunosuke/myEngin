#include "OBBCollider.h"
#include "Object/GameObject/GameObject.h"

OBBCollider::OBBCollider(Quaternion q, Vector3 extent, Vector3 center)
{
	local_extent_ = extent;
	local_center_ = center;
	local_quaternion_ = q;
	collision_type_ = CollisonType::OBB;
}


void OBBCollider::ComponentUpdate()
{
	// ローカル行列を計算
	XMMATRIX S = DirectX::XMMatrixScaling(
		local_extent_.x,
		local_extent_.y,
		local_extent_.z
	);
	XMMATRIX R = XMMatrixIdentity();

	XMMATRIX T = DirectX::XMMatrixTranslation(
		local_center_.x,
		local_center_.y,
		local_center_.z
	);

	XMMATRIX mat = S * R * T * transform->lock()->matrix;

	
	// 半径にスケール適応
	extent = Vector3::Scale(local_extent_ *0.5f, transform->lock()->scale);
	// オフセット適応
	center = mat.r[3];

	// 回転適応
	Quaternion q = transform->lock()->quaternion;
	unidirectional[0] = Vector3::Normalize(Vector3(q * Vector3::right));
	unidirectional[1] = Vector3::Normalize(Vector3(q * Vector3::up));
	unidirectional[2] = Vector3::Normalize(Vector3(q * Vector3::forward));

}

void OBBCollider::Infomation()
{
	ImGui::Checkbox("IsTrigger", &is_trigger_);
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
