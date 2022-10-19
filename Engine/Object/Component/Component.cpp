#include "Component.h"
#include "Object/Component/Transform.h"
#include "Object/GameObject/GameObject.h"

Component::~Component()
{
	if (game_object_ != nullptr)
	{
		auto component = game_object_->GetComponentList().begin();
		for (; component != game_object_->GetComponentList().end(); ++component)
		{
			if (*component == this)
			{
				game_object_->GetComponentList().erase(component);
				break;
			}
		}
	}
}

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
		if (!isDontRemove) 
		{
			if (ImGui::Button("Remove"))
			{
				Destroy(this);
			}

		}
		ImGui::TreePop();
	}
}
