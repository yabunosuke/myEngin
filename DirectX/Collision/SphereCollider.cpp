#include "SphereCollider.h"
#include "TransformComponent.h"
#include "PrimitiveRenderer.h"

using namespace DirectX;

void SphereCollider::Update()
{
	// ���[���h�s�񂩂���W�𒊏o
	XMMATRIX matWorld = XMMatrixIdentity();
	if (object->GetComponent<TransformComponent>() != nullptr) {
		matWorld = object->GetComponent<TransformComponent>()->GetMatrix();
	}
	
	// ���̃����o�ϐ����X�V
	Sphere::center = matWorld.r[3] + offset;
	Sphere::radius = radius_;
}

void SphereCollider::Draw()
{

}

void SphereCollider::Infomation()
{
	ImGui::Text("offset"); ImGui::SameLine(100); ImGui::DragFloat3("##Pos", (float *)&offset);
	ImGui::Text("radius"); ImGui::SameLine(100); ImGui::DragFloat("##Rot", (float *)&radius_);

}
