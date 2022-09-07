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
		std::weak_ptr<LightManager> light_manager,							// ライトマネージャ
		const LightType &type = LightType::Point,
		const float &range = 10.0f,
		const XMFLOAT2 &spot_angle = { 21.80208f , 30.0f},
		const XMFLOAT4 &color = {1.0f,1.0f,1.0f,1.0f},
		const float &intensity = 1.0f,
		const float &indirect_multiplier = 1.0f
		);
	void Infomation() override;

	void ComponentInitialize() override;
	void ComponentUpdate() override;

	std::weak_ptr<LightDate> GetLightDate() { return light_date_; }


private:

	// 転送用のライトデータ
	std::shared_ptr<LightDate> light_date_;


	// ライトの設定
	LightType light_type_;		// ライトの種類


	float indirect_multiplier_ = 1.0f;		// 間接光の強さ
	ShadowType shadow_type_;		// 影の種類

};

