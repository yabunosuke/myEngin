#include "Rigidbody.h"

#include "Object/GameObject/GameObject.h"
#include "Time/Time.h"

Rigidbody::Rigidbody() :
	Component("Rigidbody", ComponentType::RigitBody),
	velocity_({ 0.0f,0.0f,0.0f })
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
	ImGui::DragFloat("Drag", &drag_, 0.1f, 0.0f, 100.0f, "%.0f%%");

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
		//ImGui::DragFloat3("Angular Velocity", &velocity_.x);
		// Velocity
		//ImGui::DragFloat3("Velocity", &velocity_.x);


		ImGui::EndDisabled();

		ImGui::TreePop();
	}
}

void Rigidbody::ComponentInitialize()
{
}

void Rigidbody::ComponentFixedUpdate()
{
	/*float test_masatu_ = 0.99f;

	velocity_ *= test_masatu_;*/

	// �d�͌v�Z
	if (use_gravity_)
	{
		velocity = velocity + Vector3::down * 9.8f * Time::GetInstance()->fixedDeltaTime;
	}
	// ��R�v�Z
	velocity = velocity - ((drag_ / 100.0f) * velocity);
	transform_->position =
		transform_->position
		+ (velocity * Time::GetInstance()->fixedDeltaTime);
}

void Rigidbody::ComponentUpdate()
{
}

void Rigidbody::AddForce(Vector3 force, ForceMode force_mode)
{
	if (force.Magnitude() == 0.0f) return;
	// ���ʂ��l�����Čv�Z
	switch (force_mode)
	{
	case ForceMode::Force:
		velocity = velocity + force / mass_ * Time::GetInstance()->fixedDeltaTime;
		break;
	case ForceMode::Acceleration:
		velocity = velocity + force * Time::GetInstance()->fixedDeltaTime;
		break;
	case ForceMode::Impulse:
		velocity = velocity + force / mass_;
		break;
	case ForceMode::VelocityChange:
		velocity = velocity + force;
		break;
	default:
		break;
	}
}


