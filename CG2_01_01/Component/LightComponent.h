#pragma once
#include "Component/Component.h"

enum class TYPE
{
	DIRECTIONAL,	// 平行光源
	POINT,			// ポイントライト
	SPOT,			// スポットライト
};

class LightComponent :
    public Component
{
public:
	LightComponent(
		const TYPE &type,
		const XMFLOAT3 &color,
		const float &power,
		const float &range
		);
	void Infomation() override;


private:
	// ライトの種類
	TYPE type_;
	// 色
	XMFLOAT3 color_;
	// 強さ
	float power_;
	// 長さ
	float range_;
};

