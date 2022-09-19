#include "Behaviour.h"

Behaviour::Behaviour(const std::string &name, ComponentType component_type):
Component(name,component_type)
{
}

void Behaviour::ImGuiDraw()
{
	ImGui::Checkbox("##is_active_", &enabled_);
	if (ImGui::TreeNode(name->c_str())) {
		ImGui::Separator();
		Infomation();
		if (!isDontRemove) {
			isRemove = ImGui::Button("Remove");
		}
		ImGui::TreePop();
	}
}
