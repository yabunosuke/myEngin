#include "SphereCollider.h"
#include "Object/Component/Transform.h"
#include "PrimitiveRenderer.h"

using namespace DirectX;

void SphereCollider::Update()
{
	// ワールド行列から座標を抽出
	XMMATRIX matWorld = XMMatrixIdentity();
	if (!object.lock()->GetComponent<Transform>().expired()) {
		matWorld = object.lock()->GetComponent<Transform>().lock()->GetWorldMatrix();
	}
	
	// 球のメンバ変数を更新
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
