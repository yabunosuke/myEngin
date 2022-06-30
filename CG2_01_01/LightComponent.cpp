#include "LightComponent.h"

LightComponent::LightComponent(XMFLOAT4 color):
	Component("Light"),
	color(color)
{
}

void LightComponent::Infomation()
{
	ImGui::Text("Color\t");
	ImGui::SameLine();
	bool open_popup = ImGui::ColorButton("Light Color", ImVec4(color.x, color.y, color.z, color.w), 0, {100,0});

	ImGui::ColorPicker4("##picker", (float *)&color);

}
