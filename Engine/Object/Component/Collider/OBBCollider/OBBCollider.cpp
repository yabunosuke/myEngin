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
	// ���[�J���s����v�Z
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

	
	// ���a�ɃX�P�[���K��
	extent = Vector3::Scale(local_extent_ *0.5f, transform->lock()->scale);
	// �I�t�Z�b�g�K��
	center = mat.r[3];

	// ��]�K��
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
		// ����s�A�m�F�p
		ImGui::BeginDisabled();
		// ���ۂ̒��S
		ImGui::DragFloat3("World Center", &center.x);
		// ���ۂ̃X�P�[��
		ImGui::DragFloat3("World Extent", &extent.x);
		// �e���x�N�g��
		ImGui::DragFloat3("Unidirectional Right", &unidirectional[0].x);
		ImGui::DragFloat3("Unidirectional Up", &unidirectional[1].x);
		ImGui::DragFloat3("Unidirectional Forward", &unidirectional[2].x);

		ImGui::EndDisabled();

		ImGui::TreePop();
	}

}
