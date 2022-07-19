#include "TransformComponent.h"
#include "yMath.h"

TransformComponent::TransformComponent() :
	Component("Transform",true)
{
}

void TransformComponent::Infomation()
{
	ImGui::Text("Position"); ImGui::SameLine(100); ImGui::DragFloat3("##Pos", (float *)&transform_.position);
	ImGui::Text("Rotation"); ImGui::SameLine(100); ImGui::DragFloat3("##Rot", (float *)&transform_.rotate);
	ImGui::Text("Scale"); ImGui::SameLine(100); ImGui::DragFloat3("##Sca", (float *)&transform_.scale);
}

void TransformComponent::VirtualUpdate()
{
	// ç¿ïWïœä∑çsóÒ
	XMMATRIX S = DirectX::XMMatrixScaling(
		transform_.scale.x,
		transform_.scale.y, 
		transform_.scale.z
	);
	XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(
		DegToRad(transform_.rotate.x),
		DegToRad(transform_.rotate.y), 
		DegToRad(transform_.rotate.z)
	);
	XMMATRIX T = DirectX::XMMatrixTranslation(
		transform_.position.x, 
		transform_.position.y,
		transform_.position.z
	);
	matrix_ = S * R * T;
}