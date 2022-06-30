#pragma once
#include "Component.h"
class LightComponent :
    public Component
{
private: // �T�u�N���X
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

