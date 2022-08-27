#pragma once
#include "DirectXCommon.h"
#include "Transform.h"
#include "Component/Component.h"
#include "ConstantBufferManager/ConstantBuffer.h"

class CameraManager;


class Camera :
    public Component
{
public:
	Camera();

	void ComponentInitialize() override;
	void ComponentUpdate() override;
	void ComponentDraw() override;

	void Infomation() override;

	static void BufferTransfer(
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list,
		UINT subresource,
		UINT rootparameta_index
	);

	//===========================================
	//
	//		アクセッサ
	//
	//===========================================
	Property<std::weak_ptr<Camera>> main{
		main_camera_, AccessorType::AllAccess,
		nullptr,
		// ローカルの再計算処理
		nullptr
	};
private:

	//===========================================
	//
	//		静的メンバ変数
	//
	//===========================================

	// 現在有効なカメラ
	static std::weak_ptr<Camera> main_camera_;
	// カメラコンテナ
	static std::vector<std::weak_ptr<Camera>> cameras_;


	//===========================================
	//
	//		静的メンバ変数
	//
	//===========================================

	DirectX::XMFLOAT4 view_position;
	DirectX::XMMATRIX mat_view;
	DirectX::XMMATRIX mat_projection;

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

