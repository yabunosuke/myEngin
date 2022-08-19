#include "Object3dComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "yMath.h"

Object3dComponent::Object3dComponent(ComPtr<ID3D12Device> dev, ComPtr<ID3D12GraphicsCommandList> cmd_list, const char *file_path):
	Component("Object3d", ComponentID::Mesh, true)
{
	this->dev = dev;
	this->cmd_list = cmd_list;
	object_fbx = std::make_shared<Fbx>(dev.Get(), file_path);

}

void Object3dComponent::ComponentInitialize()
{
}

void Object3dComponent::ComponentUpdate()
{
	XMFLOAT4X4 world;
	DirectX::XMStoreFloat4x4(&world, parent->GetComponent<TransformComponent>()->GetMatrix());

	// アニメーション再生
	object_fbx->UpdateAnimation();

	// トランスフォーム
	object_fbx->UpdateTransform(world);
}

void Object3dComponent::ComponentDraw()
{
	object_fbx->Draw(dev.Get(), cmd_list.Get());
}

void Object3dComponent::ComponentFinalize()
{
}

void Object3dComponent::Infomation()
{
	// 現在選択されているアニメーション名
	static const char *combo_preview_value = nullptr;
	static int animation_id = 0;
	if (ImGui::BeginCombo("Animation ", combo_preview_value))
	{
		for (int n = 0; n < object_fbx.get()->GetResource()->GetAnimations().size(); n++) {
			auto &animation = object_fbx.get()->GetResource()->GetAnimations()[n];
			const bool is_selected = (animation_id == n);
			
			if (ImGui::Selectable(animation.name.c_str(), is_selected)) {
				animation_id = n;
				combo_preview_value = animation.name.c_str();
			}
			if (is_selected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	static bool is_loop = false;
	if (ImGui::Checkbox("isLoop", &is_loop)) {
	}

	if (ImGui::Button("Play Animation")) {
		object_fbx.get()->PlayAnimation(animation_id,is_loop,1.5f);
	}

	/*ImGui::ProgressBar(
		object_fbx.get()->GetResource()->GetAnimations()[0].seconds_length/
		object_fbx.get()->GetCurrentAnimationSeconds());*/
}
