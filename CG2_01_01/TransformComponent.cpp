#include "TransformComponent.h"


TransformComponent::TransformComponent(XMFLOAT3 position, XMFLOAT3 rotate, XMFLOAT3 scale) :
	Component("Transform",true),
	position	(position),
	rotate		(rotate),
	scale		(scale)
{
}

void TransformComponent::Infomation()
{
	ImGui::Text("Position"); ImGui::SameLine(100); ImGui::DragFloat3("##Pos", (float *)&position);
	ImGui::Text("Rotation"); ImGui::SameLine(100); ImGui::DragFloat3("##Rot", (float *)&rotate);
	ImGui::Text("Scale"); ImGui::SameLine(100); ImGui::DragFloat3("##Sca", (float *)&scale);
}

void TransformComponent::VirtualUpdate()
{
}
