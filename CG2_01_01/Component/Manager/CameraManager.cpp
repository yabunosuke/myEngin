#include "CameraManager.h"

int CameraManager::now_camera_num_ = 0;
CameraDeta *CameraManager::now_camera = nullptr;

void CameraManager::AddCamera(std::weak_ptr<CameraDeta> camera)
{
	camera_list_.emplace_back(camera);
}

void CameraManager::BufferTransfer(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list, UINT subresource, UINT rootparameta_index)
{
	// 今のカメラ
	now_camera = camera_list_[now_camera_num_].lock().get();

	// 使用しているカメラデータを格納
	CameraConstantBuffer const_camera_map;
	const_camera_map.view_position = now_camera->view_position;
	// ビュープロジェクション
	DirectX::XMMATRIX view_projection_data = now_camera->mat_view * now_camera->mat_projection;
	XMStoreFloat4x4(&const_camera_map.view_projection, view_projection_data);

	DirectX::XMStoreFloat4x4(&const_camera_map.inv_view_projection, DirectX::XMMatrixInverse(nullptr, view_projection_data));

	ConstantBufferManager::GetInstance()->BufferTransfer<CameraConstantBuffer>(
		cmd_list, subresource, rootparameta_index,
		BufferName::Camera, &const_camera_map
		);

}
