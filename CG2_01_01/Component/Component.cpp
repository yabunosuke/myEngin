#include "Component.h"

void Component::CheckInitialize()
{
	if (!isActive) return;
	ComponentInitialize();
}

void Component::CheckUpdate()
{
	if (!isActive) return;
	ComponentUpdate();
}

void Component::CheckLustUpdate()
{
	if (!isActive) return;
	ComponentLustUpdate();
}

void Component::CheckDraw()
{
	if (!isActive) return;
	ComponentDraw();
}

void Component::CheckFinalize()
{
	if (!isActive) return;
	ComponentFinalize();
}

Component::Component(std::string name, ComponentID component_id, bool dontRemove):
	name(name),
	isActive(true),
	isDontRemove(dontRemove),
	isRemove(false),
	component_id_(component_id)
{
}


void Component::ImGuiDraw()
{
	ImGui::Checkbox("##isActive", &isActive); ImGui::SameLine();
	if (ImGui::TreeNode(name.c_str())) {
		ImGui::Separator();
		Infomation();
		if (!isDontRemove) {
			isRemove = ImGui::Button("Remove");
		}
		ImGui::TreePop();
	}
	
}
