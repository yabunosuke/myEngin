#pragma once


#include "Object/Component/Transform.h"
#include "Object/Component/Component.h"
#include "ConstantBufferManager/ConstantBuffer.h"

class LightManager;

enum class LightType
{
	Spot,			// スポットライト
	Directional,	// 平行光源
	Point,			// ポイントライト
};

enum class ShadowType
{
	NoShadows,		// 影なし
	HardShadows,	// ハードシャドウ
	SoftShadows,	// ソフトシャドウ
};

class Light :
    public Component
{
public:
	Light(
		const LightType &type = LightType::Point,
		const float &range = 10.0f,
		const XMFLOAT2 &spot_angle = { 21.80208f , 30.0f},
		const XMFLOAT4 &color = {1.0f,1.0f,1.0f,1.0f},
		const float &intensity = 1.0f,
		const float &indirect_multiplier = 1.0f
		);
	~Light();


	static void SetLightManager(LightManager *light_manager);




	void Infomation() override;

	void ComponentInitialize() override;
	void ComponentUpdate() override;

	LightDate *GetLightDate() { return light_date_.get(); }

	/// <summary>
	/// カラー（AllAccess）
	/// </summary>
	yEngine::Property<XMFLOAT4> color
	{
		nullptr,
		yEngine::AccessorType::AllAccess,
		[this]() {return light_date_->color; },
		[this](XMFLOAT4 color) {light_date_->color = color; }
	};


	/// <summary>
	/// タイプ（AllAccess）
	/// </summary>
	yEngine::Property<float> intensity
	{
		nullptr,
		yEngine::AccessorType::AllAccess,
		[this]()
		{
			return light_date_->intensity;
		},
		[this](float intensity) 
		{
			light_date_->intensity = intensity;
		}
	};


	/// <summary>
	/// レンジ（AllAccess）
	/// </summary>
	yEngine::Property<float> range
	{
		nullptr,
		yEngine::AccessorType::AllAccess,
		[this]() {return light_date_->range; },
		[this](float range) {light_date_->range = range; }
	};

	/// <summary>
	/// タイプ（AllAccess）
	/// </summary>
	yEngine::Property<LightType> lighttype
	{
		nullptr,
		yEngine::AccessorType::AllAccess,
		[this]() {
			if(light_date_->light_type.x) return LightType::Spot;
			if(light_date_->light_type.y) return LightType::Directional;
			if(light_date_->light_type.z) return LightType::Point;
		},
		[this](LightType type) {
			switch (type)
			{
				case LightType::Spot:
					light_date_->light_type.x = 1;
					break;
				case LightType::Directional:
					light_date_->light_type.y = 1;
					break;
				case LightType::Point:
					light_date_->light_type.z = 1;
					break;
				default:
					break;
			}
		}
	};


private:

	/// <summary>
	/// 現在シーンに設定されているマネージャ
	/// </summary>
	static LightManager *scene_light_manager_;


	// 転送用のライトデータ
	std::unique_ptr<LightDate> light_date_;

	// ライトの設定
	LightType light_type_;		// ライトの種類


	float indirect_multiplier_ = 1.0f;		// 間接光の強さ
	ShadowType shadow_type_;		// 影の種類


};

