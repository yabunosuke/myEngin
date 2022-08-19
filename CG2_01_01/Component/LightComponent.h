#pragma once
#include "Component/Component.h"

enum class TYPE
{
	DIRECTIONAL,	// ���s����
	POINT,			// �|�C���g���C�g
	SPOT,			// �X�|�b�g���C�g
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
	// ���C�g�̎��
	TYPE type_;
	// �F
	XMFLOAT3 color_;
	// ����
	float power_;
	// ����
	float range_;
};

