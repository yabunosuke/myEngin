#include "Camera.h"
#include "Object/GameObject/GameObject.h"
#include "ConstantBufferManager/ConstantBufferManager.h"

std::weak_ptr<Camera> Camera::main_camera_;
std::vector<std::weak_ptr<Camera>> Camera::cameras_;
Property<std::weak_ptr<Camera>> Camera::main{
		main_camera_, AccessorType::ReadOnly,
		nullptr,
		nullptr
};

Camera::Camera():
	Component("Camera", ComponentType::Camera)
{


	//// �]���p�̃J�����f�[�^
	//camera_date_ = std::make_shared<CameraDeta>();
	//cameras_.emAddCamera(camera_date_);
}

void Camera::ComponentInitialize()
{
	if (main_camera_.expired()) {
		main_camera_ = std::dynamic_pointer_cast<Camera>(shared_from_this());
	}
	cameras_.emplace_back(std::dynamic_pointer_cast<Camera>(shared_from_this()));
}

void Camera::ComponentUpdate()
{
	// �J�����̈ʒu���Z�b�g
	view_position = {
		transform_.lock()->position->x,
		transform_.lock()->position->y,
		transform_.lock()->position->z,
		1.0f
	};

	// �œ_�v�Z
	XMMATRIX temp = XMMatrixRotationQuaternion(XMLoadFloat4(&transform_.lock()->quaternion));
	XMVECTOR target = {
		transform_.lock()->position->x + temp.r[2].m128_f32[0] * focus_,
		transform_.lock()->position->y + temp.r[2].m128_f32[1] * focus_,
		transform_.lock()->position->z + temp.r[2].m128_f32[2] * focus_,
		1.0f
	};

	// �r���[�s��v�Z
	switch (projection_type_)
	{
	case Camera::Perspective:
		mat_view = XMMatrixLookAtLH(
			XMLoadFloat3(&transform_.lock()->position),
			target,
			XMLoadFloat3(&Vector3::up)
		);

		break;

	case Camera::Orthographic:
		mat_view = XMMatrixOrthographicLH(
			10 * view_point_size_,
			10 * view_point_size_,
			near_plane_,
			far_plane_
		);

		break;
	default:
		break;
	}

	// �v���W�F�N�V�����s��v�Z
	mat_projection = XMMatrixPerspectiveFovLH(
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
	// �N���A�t���O�ݒ�
	{
		static const char *clear_flags_name[] =
		{
			"Skybox", "Solid Color", "Depth only"
		};
		int select_light_type = static_cast<int>(clear_flag_);
		ImGui::Combo("Clear Flags", &select_light_type, clear_flags_name, IM_ARRAYSIZE(clear_flags_name));
		clear_flag_ = static_cast<ClearFlag>(select_light_type);
	}

	// �J���[�ݒ�
	{
		ImGui::ColorEdit4("Color", &background_color_.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
	}

	// �v���W�F�N�V�����^�C�v�ݒ�
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
		// FOV�ݒ�
		ImGui::SliderFloat("Field of View", &fov_of_view_, 0.01f, 179.0f);
		break;
	case Camera::Orthographic:
		// Size�ݒ�
		ImGui::DragFloat("Size", &view_point_size_);
		break;
	default:
		break;
	}
	
	// �N���b�s���O����
	{
		ImGui::Text("Clipping Planes");
		ImGui::DragFloatRange2("range float", &near_plane_, &far_plane_, 0.03f, 0.01f, FLT_MAX, "Near: %.01f", "Far: %.01f", ImGuiSliderFlags_AlwaysClamp);

	}

}

void Camera::BufferTransfer(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list, UINT subresource, UINT rootparameta_index)
{
	// �g�p���Ă���J�����f�[�^���i�[
	CameraConstantBuffer const_camera_map;
	{
		const_camera_map.view_position = main_camera_.lock()->view_position;
		// �r���[�v���W�F�N�V����
		DirectX::XMMATRIX view_projection_data = main_camera_.lock()->mat_view * main_camera_.lock()->mat_projection;
		XMStoreFloat4x4(&const_camera_map.view_projection, view_projection_data);
		// �r���[�v���W�F�N�V�����̋t�s��
		DirectX::XMStoreFloat4x4(&const_camera_map.inv_view_projection, DirectX::XMMatrixInverse(nullptr, view_projection_data));

	}
	ConstantBufferManager::GetInstance()->BufferTransfer<CameraConstantBuffer>(
		cmd_list, subresource, rootparameta_index,
		BufferName::Camera, &const_camera_map
		);

}