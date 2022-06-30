#pragma once
#include "Component.h"

class CameraComponent :
	public Component
{
public:	//�T�u�N���X
	//�N���A�t���O
	enum class CLEAR_FLAG
	{
		SKY_BOX,
		SOILD_COLOR,
		DEPTH_ONLY,
		DONT_CLEAR
	};

	//���e���@
	enum class PROJECTION_TYPE
	{
		PERSPECTIV,
		ORTHOGRAPHIC
	};
	
public:
	CameraComponent();

private:
	CLEAR_FLAG clearFlag;
};

