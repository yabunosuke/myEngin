#include "Light.h"

#include "Object/GameObject/GameObject.h"
#include "Scene/Manager/LightManager.h"

LightManager *Light::scene_light_manager_;


Light::Light(
	const LightType &type, 
	const float &range,
	const XMFLOAT2 &spot_angle,
	const XMFLOAT4 &color,
	const float &intensity,
	const float &indirect_multiplier):
	//Component("Light", ComponentType::Light),
	light_type_(type)
{
	// 転送用のライトデータ
	light_date_ = std::make_unique<LightDate>();
	light_date_->intensity = intensity;


	// マネージャにセット
	scene_light_manager_->AddLight(light_date_.get());


	switch (type)
	{
	case LightType::Spot:

		break;
	case LightType::Directional:


	case LightType::Point:
	default:
		break;
	}
}

Light::~Light()
{
	// ライトマネージャーから削除
	if (scene_light_manager_ != nullptr &&
		scene_light_manager_->GetLightList().size() != 0)
	{
		auto light = scene_light_manager_->GetLightList().begin();
		for (; light != scene_light_manager_->GetLightList().end(); ++light)
		{
			if (*light == this->light_date_.get())
			{
				scene_light_manager_->GetLightList().erase(light);
				break;
			}
		}
	}
}

void Light::SetLightManager(LightManager *light_manager)
{
	scene_light_manager_ = light_manager;
}

void Light::Infomation()
{

	// ライトタイプ設定
	{
		static const char *light_names[] =
		{
			"Spot", "Directional", "Point"
		};
		int select_light_type = static_cast<int>(light_type_);
		ImGui::Combo("Type", &select_light_type, light_names, IM_ARRAYSIZE(light_names));
		light_type_ = static_cast<LightType>(select_light_type);
	}

	// レンジ設定
	if( light_type_ == LightType::Spot ||
		light_type_ == LightType::Point)
	{
		ImGui::DragFloat("Range", &light_date_->range, 0.02f);
	}

	// カラー設定
	{
		ImGui::ColorEdit4("Color", &light_date_->color.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
	}

	// 輝度設定
	{
		ImGui::DragFloat("Intensity", &light_date_->intensity, 0.02f,0.0f,100.0f);
	}

	// 間接光設定
	{
		ImGui::DragFloat("Indirect Multiplier", &indirect_multiplier_, 0.02f, 0.0f, 100.0f);
	}

	// 影の種類
	{
		static const char *shadow_names[] =
		{
			"No Shadows", "Hard Shadows", "Soft Shadows"
		};
		int select_shadow_type = static_cast<int>(shadow_type_);
		ImGui::Combo("Shadow Type", &select_shadow_type, shadow_names, IM_ARRAYSIZE(shadow_names));
		shadow_type_ = static_cast<ShadowType>(select_shadow_type);
	}
}

void Light::ComponentInitialize()
{
}

void Light::ComponentUpdate()
{
	// データを転送用に変換

	light_date_->position = {
		transform->lock()->position->x,
		transform->lock()->position->y,
		transform->lock()->position->z,
		1.0f
	};


	XMFLOAT4 select_light_type = {
		static_cast<int>(light_type_) == 0 ? 1.0f : 0.0f,
		static_cast<int>(light_type_) == 1 ? 1.0f : 0.0f,
		static_cast<int>(light_type_) == 2 ? 1.0f : 0.0f,
		static_cast<int>(light_type_) == 3 ? 1.0f : 0.0f
	};

	switch(light_type_)
	{
	case LightType::Directional:
	{
		light_date_->light_type = { 0.0f,1.0f,0.0f,0.0f };
		Vector3 direction_v = transform->lock()->quaternion->EulerAngles();
		Vector3::Normalize(direction_v);
		light_date_->direction = {
			direction_v.x,
			direction_v.y,
			direction_v.z,
			1.0f,
		};
	}
		break;
	case LightType::Point:
		break;
	}

}
