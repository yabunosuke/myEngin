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

	/// <summary>
	/// �J���[�iAllAccess�j
	/// </summary>
	yEngine::Property<XMFLOAT4> color
	{
		nullptr,
		yEngine::AccessorType::AllAccess,
		[this]() {return light_date_->color; },
		[this](XMFLOAT4 color) {light_date_->color = color; }
	};


	/// <summary>
	/// �^�C�v�iAllAccess�j
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
	/// �����W�iAllAccess�j
	/// </summary>
	yEngine::Property<float> range
	{
		nullptr,
		yEngine::AccessorType::AllAccess,
		[this]() {return light_date_->range; },
		[this](float range) {light_date_->range = range; }
	};

	/// <summary>
	/// �^�C�v�iAllAccess�j
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
	/// ���݃V�[���ɐݒ肳��Ă���}�l�[�W��
	/// </summary>
	static LightManager *scene_light_manager_;


	// �]���p�̃��C�g�f�[�^
	std::unique_ptr<LightDate> light_date_;

	// ���C�g�̐ݒ�
	LightType light_type_;		// ���C�g�̎��


	float indirect_multiplier_ = 1.0f;		// �Ԑڌ��̋���
	ShadowType shadow_type_;		// �e�̎��


};

