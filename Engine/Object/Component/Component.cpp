#include "Component.h"
#include "Object/Component/Transform.h"
#include "Object/GameObject/GameObject.h"



Component::Component(std::string name, ComponentType component_id, bool dontRemove):
	Object(name),
	type_(component_id),
	dont_remove_(dontRemove)
{
}

Component::~Component()
{
	if (!game_object_.expired())
	{
		auto itr = game_object_.lock()->GetComponentList().begin();
		for (; itr != game_object_.lock()->GetComponentList().end(); ++itr)
		{
			std::weak_ptr<Component> component = itr->lock();
			if (component.lock()->GetInstanceID() == weak_from_this().lock()->GetInstanceID())
			{
				game_object_.lock()->GetComponentList().erase(itr);
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

void Component::SetTransform(std::weak_ptr<Transform>trans) 
{
	transform_.swap(trans);

}



void Component::ImGuiDraw()
{
	ImGui::SameLine(30);
	if (ImGui::TreeNode(name->c_str())) {
		ImGui::Separator();
		Infomation();
		if (!dont_remove_) 
		{
			if (ImGui::Button("Remove"))
			{
				Destroy(this);
			}

		}
		ImGui::TreePop();
	}
}
