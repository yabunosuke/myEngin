#include "Component.h"
#include "Object/Component/Transform.h"

void Component::CheckInitialize()
{
	ComponentInitialize();
}

void Component::CheckFixedUpdate()
{
	ComponentFixedUpdate();
}

void Component::CheckUpdate()
{
	ComponentUpdate();
}

void Component::CheckLustUpdate()
{
	ComponentLustUpdate();
}

void Component::CheckDraw()
{
	ComponentDraw();
}

void Component::CheckFinalize()
{
	ComponentFinalize();
}

Component::Component(std::string name, ComponentType component_id, bool dontRemove):
	isDontRemove(dontRemove),
	isRemove(false),
	type_(component_id)
{


	// Object—p
	this->name = name;
	tag_ = "Notag";
}


void Component::ImGuiDraw()
{
	ImGui::SameLine(30);
	if (ImGui::TreeNode(name->c_str())) {
		ImGui::Separator();
		Infomation();
		if (!isDontRemove) {
			isRemove = ImGui::Button("Remove");
		}
		ImGui::TreePop();
	}
	
}
