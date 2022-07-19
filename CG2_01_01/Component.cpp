#include "Component.h"

void Component::Initialize()
{
	if (!isActive) return;
	VirtualInitialize();
}

void Component::Update()
{
	if (!isActive) return;
	VirtualUpdate();
}

void Component::LustUpdate()
{
	if (!isActive) return;
	VirtualLustUpdate();
}

void Component::Draw()
{
	if (!isActive) return;
	VirtualDraw();
}

void Component::Finalize()
{
	if (!isActive) return;
	VirtualFinalize();
}

Component::Component(std::string name, bool dontRemove):
	name(name),
	isActive(true),
	isDontRemove(dontRemove),
	isRemove(false)
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
