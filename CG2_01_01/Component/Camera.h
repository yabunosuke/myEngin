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
		std::weak_ptr<CameraManager> camera_manager
	);

	void ComponentInitialize() override;
	void ComponentUpdate() override;
	void ComponentDraw() override;

	void Infomation() override;

private:
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

	enum ProjectionType			// 投影方法
	{
		Perspective,			// 透視投影変換
		Orthographic			// 平衡投影変換
	}projection_type_;

	float fov_of_view_		{ 60.0f };			// カメラのビュー角度
	float view_point_size_	{ 5.0f };			// Orthographic設定した場合のサイズ
	float near_plane_		{ 0.3f };			// 描画される最も近い点
	float far_plane_		{ 1000.0f };		// 描画される最も遠い点
	float focus_			{ 1.0f };			// 焦点距離;
};

