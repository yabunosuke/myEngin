#pragma once
#include "Component.h"

class CameraComponent :
	public Component
{
public:	//サブクラス
	//クリアフラグ
	enum class ClearFlag
	{
		SKY_BOX,
		SOILD_COLOR,
		DEPTH_ONLY,
		DONT_CLEAR
	};

	//投影方法
	enum class ProjectionType
	{
		PERSPECTIV,
		ORTHOGRAPHIC
	};
	
	
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	CameraComponent();

	void VirtualUpdate() override;

private:
	ClearFlag clearFlag;

	XMFLOAT3 eye_;
	XMFLOAT3 target_;
	XMFLOAT3 up_;
};

