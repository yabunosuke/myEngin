#pragma once
#include "Component/Component.h"

enum class LightType
{
	Directional,	// ���s����
	Point,			// �|�C���g���C�g
	Spot,			// �X�|�b�g���C�g
};

enum class ShadowType
{
	NoShadows,		// �e�Ȃ�
	HardShadows,	// �n�[�h�V���h�E
	SoftShadows,	// �\�t�g�V���h�E
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
	// ���C�g�̐ݒ�
	// ���C�g�̎��
	LightType type_;
	// ����
	float range_;
	// �X�|�b�g���C�g�̒�̊p�x
	XMFLOAT2 spot_angle_;
	// ���C�g�̐F
	XMFLOAT4 color_;
	// �P�x
	float intensity_;
	// �Ԑڌ��̋���
	float indirect_multiplier_;
	// �e�̐ݒ�

};

