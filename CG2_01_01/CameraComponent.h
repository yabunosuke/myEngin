#pragma once
#include "Component.h"

class CameraComponent :
	public Component
{
public:	//サブクラス
	//クリアフラグ
	enum class CLEAR_FLAG
	{
		SKY_BOX,
		SOILD_COLOR,
		DEPTH_ONLY,
		DONT_CLEAR
	};

	//投影方法
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

