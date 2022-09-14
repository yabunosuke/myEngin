#include "SphereCollider.h"
#include "Object/Component/Transform.h"
#include "PrimitiveRenderer.h"

using namespace DirectX;

void SphereCollider::Update()
{
	// ���[���h�s�񂩂���W�𒊏o
	XMMATRIX matWorld = XMMatrixIdentity();
	if (!object.lock()->GetComponent<Transform>().expired()) {
		matWorld = object.lock()->GetComponent<Transform>().lock()->GetWorldMatrix();
	}
	
	// ���̃����o�ϐ����X�V
	center = matWorld.r[3] + offset;
	radius = radius_;
}

void SphereCollider::Draw()
{

}

void SphereCollider::Infomation()
{
	ImGui::Text("offset"); ImGui::SameLine(100); ImGui::DragFloat3("##Pos", (float *)&offset);
	ImGui::Text("radius"); ImGui::SameLine(100); ImGui::DragFloat("##Rot", (float *)&radius_);

}
