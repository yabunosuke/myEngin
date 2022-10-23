#pragma once


#include "Object/Component/Transform.h"
#include "Object/Component/Component.h"
#include "ConstantBufferManager/ConstantBuffer.h"

class LightManager;

enum class LightType
{
	Spot,			// �X�|�b�g���C�g
	Directional,	// ���s����
	Point,			// �|�C���g���C�g
};

enum class ShadowType
{
	NoShadows,		// �e�Ȃ�
	HardShadows,	// �n�[�h�V���h�E
	SoftShadows,	// �\�t�g�V���h�E
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

private:

	/// <summary>
	/// ���݃V�[���ɐݒ肳��Ă���}�l�[�W��
	/// </summary>
	static LightManager *scene_light_manager_;

	// �]���p�̃��C�g�f�[�^
	//std::shared_ptr<LightDate> light_date_;
	std::unique_ptr<LightDate> light_date_;

	// ���C�g�̐ݒ�
	LightType light_type_;		// ���C�g�̎��


	float indirect_multiplier_ = 1.0f;		// �Ԑڌ��̋���
	ShadowType shadow_type_;		// �e�̎��


};

