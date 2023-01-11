#include "Camera.h"
#include "../Engine/Scene/Manager/CameraManager.h"
#include "Object/GameObject/GameObject.h"
#include "ConstantBufferManager/ConstantBufferManager.h"

Camera* Camera::main_camera_;
CameraManager *Camera::camera_manager_;

yEngine::Property<Camera*> Camera::main{
		&main_camera_, yEngine::AccessorType::ReadOnly,
		nullptr,
		nullptr
};

Camera::Camera():
	Component("Camera", ComponentType::Camera)
{
}

Camera::~Camera()
{
}

void Camera::ComponentInitialize()
{
	// メインカメラがなければセットする
	if (camera_manager_->main_camera_ == nullptr) {
		camera_manager_->main_camera_ = this;
		main_camera_ = camera_manager_->main_camera_;
	}
	camera_manager_->cameras_.emplace_back(this);
}

void Camera::ComponentUpdate()
{
	// カメラの位置をセット
	view_position = {
		transform->lock()->position->x,
		transform->lock()->position->y,
		transform->lock()->position->z,
		1.0f
	};

	// 焦点計算
	XMMATRIX temp = XMMatrixRotationQuaternion(XMLoadFloat4(&transform->lock()->quaternion));
	XMVECTOR target
	{
			transform->lock()->position->x + temp.r[2].m128_f32[0] * focus_,
			transform->lock()->position->y + temp.r[2].m128_f32[1] * focus_,
			transform->lock()->position->z + temp.r[2].m128_f32[2] * focus_,
			1.0f
	};
	// ビュー行列計算
	switch (projection_type_)
	{
	case Camera::Perspective:
		view_matrix = XMMatrixLookAtLH(
			XMLoadFloat3(&transform->lock()->position),
			target,
			XMLoadFloat3(&Vector3::up)
		);

		break;

	case Camera::Orthographic:
		view_matrix = XMMatrixOrthographicLH(
			10 * view_point_size_,
			10 * view_point_size_,
			near_plane_,
			far_plane_
		);

		break;
	default:
		break;
	}

	// プロジェクション行列計算
	projection_matrix = XMMatrixPerspectiveFovLH(
		fov_of_view_ * Mathf::deg_to_rad,
		static_cast<float>(WinApp::windowWidth) / static_cast<float>(WinApp::windowHeight),
		near_plane_,far_plane_
	);


}

void Camera::ComponentDraw()
{
}

void Camera::Infomation()
{
	// クリアフラグ設定
	{
		static const char *clear_flags_name[] =
		{
			"Skybox", "Solid Color", "Depth only"
		};
		int select_light_type = static_cast<int>(clear_flag_);
		ImGui::Combo("Clear Flags", &select_light_type, clear_flags_name, IM_ARRAYSIZE(clear_flags_name));
		clear_flag_ = static_cast<ClearFlag>(select_light_type);
	}

	// カラー設定
	{
		ImGui::ColorEdit4("Color", &background_color_.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
	}

	// プロジェクションタイプ設定
	{
		static const char *projection_type_name[] =
		{
			"Perspective", 	"Orthographic"
		};
		int select_projection_type = static_cast<int>(projection_type_);
		ImGui::Combo("Projection", &select_projection_type, projection_type_name, IM_ARRAYSIZE(projection_type_name));
		projection_type_ = static_cast<ProjectionType>(select_projection_type);
	}

	
	switch (projection_type_)
	{
	case Camera::Perspective:
		// FOV設定
		ImGui::SliderFloat("Field of View", &fov_of_view_, 0.01f, 179.0f);
		break;
	case Camera::Orthographic:
		// Size設定
		ImGui::DragFloat("Size", &view_point_size_);
		break;
	default:
		break;
	}
	
	// クリッピング距離
	{
		ImGui::Text("Clipping Planes");
		ImGui::DragFloatRange2("range float", &near_plane_, &far_plane_, 0.03f, 0.01f, FLT_MAX, "Near: %.01f", "Far: %.01f", ImGuiSliderFlags_AlwaysClamp);

	}

}
//
//void Camera::BufferTransfer(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list, UINT subresource, UINT rootparameta_index)
//{
//
//	// 使用しているカメラデータを格納
//	CameraConstantBuffer const_camera_map;
//	{
//		const_camera_map.view_position = main_camera_->view_position;
//		// ビュープロジェクション
//		DirectX::XMMATRIX view_projection_data = main_camera_->view_matrix * main_camera_->projection_matrix;
//		XMStoreFloat4x4(&const_camera_map.view_projection, view_projection_data);
//		// ビュープロジェクションの逆行列
//		DirectX::XMStoreFloat4x4(&const_camera_map.inv_view_projection, DirectX::XMMatrixInverse(nullptr, view_projection_data));
//
//	}
//	ConstantBufferManager::GetInstance()->BufferTransfer<CameraConstantBuffer>(
//		cmd_list, subresource, rootparameta_index,
//		BufferName::Camera, &const_camera_map
//		);
//
//}

void Camera::SetCameraManager(CameraManager *camera_manager)
{
	camera_manager_ = camera_manager;
	main_camera_ = camera_manager_->main_camera_;
}
