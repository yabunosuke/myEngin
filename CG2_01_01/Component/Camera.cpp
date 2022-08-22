#include "Camera.h"

#include "GameObject.h"
#include "Component/Manager/CameraManager.h"


Camera::Camera(
	std::weak_ptr<CameraManager> light_manager

):
	Component("Camera", ComponentID::Camera)
{

	// �]���p�̃J�����f�[�^
	camera_date_ = std::make_shared<CameraDeta>();
	light_manager.lock()->AddCamera(camera_date_);
}

void Camera::ComponentInitialize()
{
	transform_ = object_->GetComponent<Transform>();
}

void Camera::ComponentUpdate()
{
	// �J�����̈ʒu���Z�b�g
	camera_date_->view_position = {
		transform_->world_position_.x,
		transform_->world_position_.y,
		transform_->world_position_.z,
		1.0f
	};

	// �œ_�v�Z
	XMMATRIX temp = XMMatrixRotationQuaternion(XMLoadFloat4(&transform_->world_quaternion_));
	XMVECTOR target = {
		transform_->world_position_.x + temp.r[2].m128_f32[0] * focus_,
		transform_->world_position_.y + temp.r[2].m128_f32[1] * focus_,
		transform_->world_position_.z + temp.r[2].m128_f32[2] * focus_,
		1.0f
	};

	// �r���[�s��v�Z
	camera_date_ ->mat_view = XMMatrixLookAtLH(
		XMLoadFloat3(&transform_->world_position_),
		target,
		XMLoadFloat3(&Vector3::up)
	);

	// �v���W�F�N�V�����s��v�Z
	camera_date_-> mat_projection = XMMatrixPerspectiveFovLH(
		fov_of_view_ * Mathf::deg_to_rad,
		static_cast<float>(WinApp::windowWidth) / static_cast<float>(WinApp::windowHeight),
		near_,far_
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
		ImGui::Combo("Type", &select_light_type, clear_flags_name, IM_ARRAYSIZE(clear_flags_name));
		clear_flag_ = static_cast<ClearFlag>(select_light_type);
	}

	// �J���[�ݒ�
	{
		ImGui::ColorEdit4("Color", &background_color_.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
	}

	// FOV�ݒ�
	{
		ImGui::SliderFloat("Field of View", &fov_of_view_, 0.01f, 179.0f);
	}

}
