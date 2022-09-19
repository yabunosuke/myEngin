#include "Rigidbody.h"

#include "Object/GameObject/GameObject.h"
#include "Time/Time.h"

Rigidbody::Rigidbody() :
	Component("Rigidbody", ComponentType::RigitBody),
	velocity_({ 0,0,0 })
{
	
}


void Rigidbody::Infomation()
{

	// ���Z�b�g�{�^��
	if (ImGui::Button("Reset"))
	{
		velocity_= { 0,0,0 };		// ���W
	}


	// ����
	ImGui::DragFloat("Mass", &mass_,0.03f);
	// �͂œ����ꍇ�̋�C��R
	ImGui::DragInt("Drag", &drag_, 1, 0, 100, "%d%%");

	// �g���N�œ����ꍇ�̋�C��R
	ImGui::DragFloat("Angluar Drag", &angular_drag_, 0.03f);
	// �d��
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
		// ����s�A�m�F�p
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

void Rigidbody::ComponentInitialize()
{
}

void Rigidbody::ComponentFixedUpdate()
{
	float test_masatu_ = 1.0f;

	velocity_ *= test_masatu_;

	// �d�͌v�Z
	if (use_gravity_)
	{
		velocity_ += Vector3::down * 9.8f * Time::GetInstance()->fixedDeltaTime;
	}

	// ��R�v�Z
	velocity_ -= static_cast<float>(drag_ / 100.0f) * velocity_;
	transform_.lock()->localPosition =
		transform_.lock()->localPosition
		+ velocity_ * Time::GetInstance()->fixedDeltaTime;
}

void Rigidbody::ComponentUpdate()
{
}

void Rigidbody::AddForce(XMFLOAT3 force, ForceMode force_mode)
{
	// ���ʂ��l�����Čv�Z
	if( force_mode == ForceMode::Force ||
		force_mode == ForceMode::Impulse)
	{
		velocity_ += force / mass_;

	}
	// ���ʂ𖳎����Čv�Z
	else
	{
		velocity_ += force;
	}

}


