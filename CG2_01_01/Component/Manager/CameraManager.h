#pragma once
#include <memory>
#include "ConstantBufferManager/ConstantBufferManager.h"

class CameraManager
{
public:
	// カメラをリストに追加
	void AddCamera(std::weak_ptr<CameraDeta> camera);

	/// <summary>
	/// カメラをバッファに転送
	/// </summary>
	void BufferTransfer(
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list,
		UINT subresource,
		UINT rootparameta_index
	);

	
	static void SetCameraNum(int num);
	static DirectX::XMMATRIX GetProjection() { return  now_camera->mat_projection; }
	static DirectX::XMMATRIX GetView() { return  now_camera->mat_view; }
private:
	std::vector<std::weak_ptr<CameraDeta>> camera_list_;
	static int now_camera_num_;
	static CameraDeta *now_camera;
};

