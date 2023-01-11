#include "Behaviour.h"

Behaviour::Behaviour(const std::string &name, ComponentType component_type)
	/*:
	Component(name, component_type)*/
{
}

void Behaviour::ComponentFixedUpdate()
{
	if (!enabled_) return;
	Component::ComponentFixedUpdate();
}

void Behaviour::ComponentUpdate()
{
	if (!enabled_) return;
	Component::ComponentUpdate();
}

void Behaviour::ImGuiDraw()
{
	ImGui::Checkbox("##is_active_", &enabled_);
	Component::ImGuiDraw();
}
