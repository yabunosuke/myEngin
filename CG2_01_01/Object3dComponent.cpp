#include "Object3dComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "yMath.h"

Object3dComponent::Object3dComponent(ComPtr<ID3D12Device> dev, ComPtr<ID3D12GraphicsCommandList> cmd_list, const char *file_path):
	Component("Object3d", true)
{
	this->dev = dev;
	this->cmd_list = cmd_list;
	object_fbx = std::make_shared<Fbx>(dev.Get(), file_path);

}

void Object3dComponent::VirtualInitialize()
{
}

void Object3dComponent::VirtualUpdate()
{
	// オブジェクトからトランスフォーム取得
	XMFLOAT3 sca =
		parent->GetComponent<TransformComponent>()->GetScale();
	XMFLOAT3 rot =
		parent->GetComponent<TransformComponent>()->GetRotate();
	XMFLOAT3 pos =
		parent->GetComponent<TransformComponent>()->GetPositon();

	XMMATRIX S = DirectX::XMMatrixScaling(
		sca.x, sca.y, sca.z
	);
	XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(
		DegToRad(rot.x), DegToRad(rot.y), DegToRad(rot.z)
	);
	XMMATRIX T = DirectX::XMMatrixTranslation(
		pos.x, pos.y, pos.z
	);

	XMFLOAT4X4 world;
	XMMATRIX mat = S * R * T;
	DirectX::XMStoreFloat4x4(&world, mat);

	// アニメーション再生
	object_fbx->UpdateAnimation();

	// トランスフォーム
	object_fbx->UpdateTransform(world);
}

void Object3dComponent::VirtualDraw()
{
	object_fbx->Draw(dev.Get(), cmd_list.Get());
}

void Object3dComponent::VirtualFinalize()
{
}

void Object3dComponent::Infomation()
{
	/*ImGui::ProgressBar(
		object_fbx.get()->GetResource()->GetAnimations()[0].seconds_length/
		object_fbx.get()->GetCurrentAnimationSeconds());*/
}
