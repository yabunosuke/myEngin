#include "CameraManager.h"
#include <ConstantBufferManager/ConstantBufferManager.h>
#include "WinApp.h"
#include "Texture.h"
#include "PipelineManager.h"

void CameraManager::BufferTransfer(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list, UINT subresource, UINT rootparameta_index)
{
	// 使用しているカメラデータを格納
	CameraConstantBuffer const_camera_map;
	{
		const_camera_map.view_position = Camera::main.r_->viewPosition;
		// ビュープロジェクション
		DirectX::XMMATRIX view_projection_data = Camera::main.r_->viewMatrix.r_ * Camera::main.r_->projectionMatrix.r_;
		XMStoreFloat4x4(&const_camera_map.view_projection, view_projection_data);
		// ビュープロジェクションの逆行列
		DirectX::XMStoreFloat4x4(&const_camera_map.inv_view_projection, DirectX::XMMatrixInverse(nullptr, view_projection_data));

	}
	ConstantBufferManager::GetInstance()->BufferTransfer<CameraConstantBuffer>(
		cmd_list, subresource, rootparameta_index,
		BufferName::Camera, &const_camera_map
		);

}
