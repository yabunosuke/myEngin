#include "Component.h"
#include "Component/Transform.h"

void Component::CheckInitialize()
{
	if (!is_active_) return;
	ComponentInitialize();
}

void Component::CheckUpdate()
{
	if (!is_active_) return;
	ComponentUpdate();
}

void Component::CheckLustUpdate()
{
	if (!is_active_) return;
	ComponentLustUpdate();
}

void Component::CheckDraw()
{
	if (!is_active_) return;
	ComponentDraw();
}

void Component::CheckFinalize()
{
	if (!is_active_) return;
	ComponentFinalize();
}

Component::Component(std::string name, ComponentID component_id, bool dontRemove):
	is_active_(true),
	isDontRemove(dontRemove),
	isRemove(false),
	component_id_(component_id)
{


	// Object—p
	name_ = name;
	tag_ = "Notag";
}


void Component::ImGuiDraw()
{
	ImGui::Checkbox("##is_active_", &is_active_); ImGui::SameLine();
	if (ImGui::TreeNode(name_.c_str())) {
		ImGui::Separator();
		Infomation();
		if (!isDontRemove) {
			isRemove = ImGui::Button("Remove");
		}
		ImGui::TreePop();
	}
	
}
