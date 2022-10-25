#pragma once
#include "DirectXCommon.h"
#include "Transform.h"
#include "Object/Component/Component.h"
#include "ConstantBufferManager/ConstantBuffer.h"

class CameraManager;


class Camera :
    public Component
{
public:
	//===========================================
	//
	//		コンストラクタ
	//
	//===========================================

	Camera();
	~Camera();

	void ComponentInitialize() override;
	void ComponentUpdate() override;
	void ComponentDraw() override;

	void Infomation() override;

	//===========================================
	//
	//		静的関数
	//
	//===========================================

	static void SetCameraManager(CameraManager *camera_manager);

	//===========================================
	//
	//		アクセッサ
	//
	//===========================================

	//static DirectX::XMMATRIX GetProjection() { return  now_camera->projection_matrix; }
	//static DirectX::XMMATRIX GetView() { return  now_camera->view_matrix; }

	
	static yEngine::Property<Camera*> main;

	DirectX::XMFLOAT4 view_position;
	DirectX::XMMATRIX view_matrix;
	DirectX::XMMATRIX projection_matrix;
	yEngine::Property<DirectX::XMFLOAT4> viewPosition{
		&view_position, yEngine::AccessorType::AllAccess,
		nullptr,
		nullptr
	};
	yEngine::Property<DirectX::XMMATRIX> viewMatrix{
		&view_matrix,
		yEngine::AccessorType::AllAccess,
		nullptr,
		nullptr
	};
	yEngine::Property<DirectX::XMMATRIX> projectionMatrix{
		&projection_matrix, yEngine::AccessorType::AllAccess,
		nullptr,
		nullptr
	};

private:

	//===========================================
	//
	//		静的メンバ変数
	//
	//===========================================

	// 現在有効なカメラ（カメラコンテナの中で一番上にある有効なカメラ）
	static Camera* main_camera_;
	// カメラコンテナ
	static CameraManager *camera_manager_;

	//===========================================
	//
	//		メンバ変数
	//
	//===========================================


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
	}projection_type_ = Perspective;

	float fov_of_view_		{ 60.0f };			// カメラのビュー角度
	float view_point_size_	{ 5.0f };			// Orthographic設定した場合のサイズ
	float near_plane_		{ 0.3f };			// 描画される最も近い点
	float far_plane_		{ 1000.0f };		// 描画される最も遠い点
	float focus_			{ 1.0f };			// 焦点距離;

};

