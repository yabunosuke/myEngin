#include "Rigidbody.h"

#include "Object/GameObject/GameObject.h"
#include "Time/Time.h"

Rigidbody::Rigidbody() :
	Component("Rigidbody", ComponentType::RigitBody)
{
	
}


void Rigidbody::Infomation()
{

	// リセットボタン
	if (ImGui::Button("Reset"))
	{
		velocity_= { 0,0,0 };		// 座標
	}


	// 質量
	ImGui::DragFloat("Mass", &mass_,0.03f);
	// 力で動く場合の空気抵抗
	ImGui::DragFloat("Drag", &drag_, 0.1f, 0.0f, 100.0f, "%.0f%%");

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
		//
		float speed = velocity->Magnitude();
		ImGui::DragFloat("Speed", &speed);
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

	// 重力計算
	
	transform_->position += (velocity * Time::GetInstance()->fixedDeltaTime);
	// 抵抗計算
	velocity = velocity - ((drag_ / 100.0f) * velocity);
	if (use_gravity_)
	{
		velocity = velocity + Vector3::down * 9.8f * Time::GetInstance()->fixedDeltaTime;
	}

}

void Rigidbody::ComponentUpdate()
{
}

void Rigidbody::AddForce(const Vector3 &force, ForceMode force_mode)
{
	if (force.Magnitude() == 0.0f) return;
	// 質量を考慮して計算
	switch (force_mode)
	{
	case ForceMode::Force:
		velocity += force / mass_ * Time::GetInstance()->fixedDeltaTime;
		break;
	case ForceMode::Acceleration:
		velocity += force * Time::GetInstance()->fixedDeltaTime;
		break;
	case ForceMode::Impulse:
		velocity += force / mass_;
		break;
	case ForceMode::VelocityChange:
		velocity += force;
		break;
	default:
		break;
	}
}

void Rigidbody::AddForce(float x, float y, float z, ForceMode force_mode)
{
	Vector3 force{ x,y,z };
	AddForce(force, force_mode);
}

bool Rigidbody::IsSleep()
{
	// 運動エネルギー
	float e = 0.5f * mass_ * pow(velocity_.Magnitude(), 2.0f);
	return e < sleep_threshold_;
}


