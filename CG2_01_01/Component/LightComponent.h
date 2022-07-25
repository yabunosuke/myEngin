#pragma once
#include "Component/Component.h"

class LightComponent :
    public Component
{
private: // サブクラス
	enum class TYPE
	{
		SPOT,
		DIRECTIONAL,
		POINT
	};
public:
	LightComponent(XMFLOAT4 color = {1,1,1,1});
	void Infomation() override;


private:
	XMFLOAT4 color;
	TYPE type;
};

