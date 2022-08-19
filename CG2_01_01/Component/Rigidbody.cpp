#include "Component/Rigidbody.h"

void Rigidbody::Infomation()
{
	// 質量
	ImGui::DragFloat("Mass", &mass_,0.03f);
	// 力で動く場合の空気抵抗
	ImGui::DragFloat("Drag", &drag_,0.03f);
	// トルクで動く場合の空気抵抗
	ImGui::DragFloat("Angluar Drag", &angular_drag_, 0.03f);
	// 重力
	ImGui::Checkbox("Use Gravity", &use_gravity_);


	if (ImGui::TreeNode("Constraints")) {

		ImGui::Text("Freeze Position");
		ImGui::Checkbox("X##FreezePos", &freeze_position_.x); ImGui::SameLine();
		ImGui::Checkbox("Y##FreezePos", &freeze_position_.y); ImGui::SameLine();
		ImGui::Checkbox("Z##FreezePos", &freeze_position_.z);

		ImGui::Text("Freeze Rotation");
		ImGui::Checkbox("X##FreezeRot", &freeze_rotation_.x); ImGui::SameLine();
		ImGui::Checkbox("Y##FreezeRot", &freeze_rotation_.y); ImGui::SameLine();
		ImGui::Checkbox("Z##FreezeRot", &freeze_rotation_.z);

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Info")) {
		// 操作不可、確認用
		ImGui::BeginDisabled();
		// Velocity
		ImGui::DragFloat3("Velocity", &velocity_.x);
		// Angular Velocity
		ImGui::DragFloat3("Angular Velocity", &velocity_.x);
		// Velocity
		ImGui::DragFloat3("Velocity", &velocity_.x);


		ImGui::EndDisabled();

		ImGui::TreePop();
	}
}

Rigidbody::Rigidbody() :
	Component("Rigidbody", ComponentID::RigitBody)
{
}
