#pragma once
#include "Component/Component.h"

enum class LightType
{
	Directional,	// 平行光源
	Point,			// ポイントライト
	Spot,			// スポットライト
};

enum class ShadowType
{
	NoShadows,		// 影なし
	HardShadows,	// ハードシャドウ
	SoftShadows,	// ソフトシャドウ
};

class LightComponent :
    public Component
{
public:
	LightComponent(
		const LightType &type = LightType::Point,
		const float &range = 10.0f,
		const XMFLOAT2 &spot_angle = { 21.80208f , 30.0f},
		const XMFLOAT4 &color = {1.0f,1.0f,1.0f,1.0f},
		const float &intensity = 1.0f,
		const float &indirect_multiplier = 1.0f
		);
	void Infomation() override;


private:
	// ライトの設定
	// ライトの種類
	LightType type_;
	// 長さ
	float range_;
	// スポットライトの底の角度
	XMFLOAT2 spot_angle_;
	// ライトの色
	XMFLOAT4 color_;
	// 輝度
	float intensity_;
	// 間接光の強さ
	float indirect_multiplier_;
	// 影の設定

};

