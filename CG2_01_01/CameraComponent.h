#pragma once
#include "Component.h"

class CameraComponent :
	public Component
{
public:	//�T�u�N���X
	//�N���A�t���O
	enum class ClearFlag
	{
		SKY_BOX,
		SOILD_COLOR,
		DEPTH_ONLY,
		DONT_CLEAR
	};

	//���e���@
	enum class ProjectionType
	{
		PERSPECTIV,
		ORTHOGRAPHIC
	};
	
	
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CameraComponent();

	void VirtualUpdate() override;

private:
	ClearFlag clearFlag;

	XMFLOAT3 eye_;
	XMFLOAT3 target_;
	XMFLOAT3 up_;
};

