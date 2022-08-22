#pragma once
#include "Transform.h"
#include "Component/Component.h"
#include "ConstantBufferManager/ConstantBuffer.h"

class CameraManager;


class Camera :
    public Component
{
public:
	Camera(
		std::weak_ptr<CameraManager> light_manager
	);

	void ComponentInitialize() override;
	void ComponentUpdate() override;
	void ComponentDraw() override;

	void Infomation() override;

private:

	Transform *transform_;

	// 転送用のカメラデータ
	std::shared_ptr<CameraDeta> camera_date_;

	// 画面クリア方法
	enum class ClearFlag
	{
		SkyBox,			
		SoildColor,
		DepthOnly,
		DontColor
	}clear_flag_;
	
	XMFLOAT4 background_color_	{ 0,0,1,1 };	// 背景の色

	float fov_of_view_	{ 60.f };			// カメラのビュー角度
	float near_			{ 0.3f };			// 描画される最も近い点
	float far_			{ 1000.0f };		// 描画される最も遠い点
	float focus_		{ 1.0f };			// 焦点距離;
};

