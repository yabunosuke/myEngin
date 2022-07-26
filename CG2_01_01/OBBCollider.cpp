#include "Collider/OBBCollider.h"
#include  "PrimitiveRenderer.h"
#include "DirectXCommon.h"
void OBBCollider::Update()
{
	XMMATRIX matWorld = XMMatrixIdentity();
	if (object->GetComponent<TransformComponent>() != nullptr) {
		matWorld = object->GetComponent<TransformComponent>()->GetMatrix();
	}

	OBB::center = matWorld.r[3] + XMLoadFloat3(&offset_);
	
	OBB::scale = scale_;
}

void OBBCollider::Draw()
{
	PrimitiveRenderer::Box box = {
		{center.m128_f32[0],center.m128_f32[1],center.m128_f32[2]},
		{ 0,0,0},
		scale_
	};
	PrimitiveRenderer::GetInstance().DrawBox(DirectXCommon::cmdList, box);
}



void OBBCollider::Infomation()
{
	ImGui::Text("offset"); ImGui::SameLine(100); ImGui::DragFloat3("##Pos", (float *)&offset_);
	ImGui::Text("scale"); ImGui::SameLine(100); ImGui::DragFloat3("##Sca", (float *)&scale_);

}
