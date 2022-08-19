#include "LightComponent.h"


LightComponent::LightComponent(const TYPE &type, const XMFLOAT3 &color, const float &power, const float &range) :
	Component("LIGHT", ComponentID::None),
	type_(type),
	color_(color),
	power_(power),
	range_(range)
{
}

void LightComponent::Infomation()
{
	/*ImGui::Text("Color\t");
	ImGui::SameLine();
	bool open_popup = ImGui::ColorButton("LIGHT Color", ImVec4(color.x, color.y, color.z, color.w), 0, {100,0});

	ImGui::ColorPicker4("##picker", (float *)&color);*/

}
