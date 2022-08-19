#include "LightComponent.h"



LightComponent::LightComponent(const LightType &type, const float &range, const XMFLOAT2 &spot_angle, const XMFLOAT4 &color, const float &intensity, const float &indirect_multiplier):
	Component("Light", ComponentID::LIGHT),
	type_(type),
	range_(range),
	spot_angle_(spot_angle),
	color_(color),
	intensity_(intensity),
	indirect_multiplier_(indirect_multiplier)
{
}

void LightComponent::Infomation()
{
	// ���C�g�̐ݒ�
	if (ImGui::TreeNode("General")) {


		ImGui::TreePop();
	}

	// ���o
	if (ImGui::TreeNode("Emission")) {
		bool open_popup = ImGui::ColorButton("LIGHT Color", ImVec4(color_.x, color_.y, color_.z, color_.w), 0, { 100,0 });

		ImGui::ColorPicker4("##picker", (float *)&color_);

		// �P�x
		ImGui::DragFloat("Intesity", &intensity_);

		// �Ԑڌ��̋���
		ImGui::DragFloat("Indirect Multiplier", &indirect_multiplier_);

		// ����
		if(type_ == LightType::Point &&
			type_ == LightType::Spot)
		{
			ImGui::DragFloat("Range", &range_);

		}

		ImGui::TreePop();
	}
	// �e
	if (ImGui::TreeNode("Shadowing")) {
		ImGui::TreePop();
	}
	
}
