#include "Light.h"

#include "GameObject.h"
#include "Component/Manager/LightManager.h"


Light::Light(
	std::weak_ptr<LightManager> light_manager,							// ライトマネージャ
	const LightType &type, const float &range, const XMFLOAT2 &spot_angle, const XMFLOAT4 &color, const float &intensity, const float &indirect_multiplier):
	Component("Light", ComponentID::Light),
	light_type_(type)
{
	// 転送用のライトデータ
	light_date_ = std::make_shared<LightDate>();

	// マネージャをセット
	light_manager_ = light_manager;
	// マネージャにライトを登録
	light_manager_.lock()->AddLight(light_date_);
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
		ImGui::DragFloat("Intensity", &light_date_->intensity, 0.02f,0.0f,10.0f);
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

void Light::ComponentUpdate()
{
	// データを転送用に変換

	// 座標取得
	XMFLOAT3 object_pos = object_->GetComponent<Transform>()->world_position_;
	light_date_->position = {
		object_pos.x,
		object_pos.y,
		object_pos.z,
		1.0f
	};


	XMFLOAT4 select_light_type = {
		static_cast<int>(light_type_) == 0 ? 1.0f : 0.0f,
		static_cast<int>(light_type_) == 1 ? 1.0f : 0.0f,
		static_cast<int>(light_type_) == 2 ? 1.0f : 0.0f,
		static_cast<int>(light_type_) == 3 ? 1.0f : 0.0f
	};

	//// バッファ転送用のデータに変換
	//light_data_ ={
	//	// 共通項目
	//	select_light_type,	// ライトのタイプ
	//	color_,				// ライトの色

	//	// 一部共通項目
	//	object_->GetComponent<Transform>()

	//	// ポイントライト用項目


	//};
}
