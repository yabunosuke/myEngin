#include "TransformComponent.h"


TransformComponent::TransformComponent(XMFLOAT3 position, XMFLOAT3 rotate, XMFLOAT3 scale) :
	Component("Transform",true),
	position_	(position),
	rotate_		(rotate),
	scale_		(scale)
{
}

void TransformComponent::Infomation()
{
	ImGui::Text("Position"); ImGui::SameLine(100); ImGui::DragFloat3("##Pos", (float *)&position_);
	ImGui::Text("Rotation"); ImGui::SameLine(100); ImGui::DragFloat3("##Rot", (float *)&rotate_);
	ImGui::Text("Scale"); ImGui::SameLine(100); ImGui::DragFloat3("##Sca", (float *)&scale_);
}

void TransformComponent::VirtualUpdate()
{
}

DirectX::XMFLOAT4X4 TransformComponent::GetTransform() const
{
	return XMFLOAT4X4();
}
