#include "PlayerCameraController.h"
#include "Input.h"
#include "Math/Easing.h"

PlayerCameraController::PlayerCameraController() :
	MonoBehaviour("PlayerCameraController")
{
}

void PlayerCameraController::Start()
{
	rotation_root_ = GameObject::Find("RotationRoot")->transform_;
	height_root_ = GameObject::Find("HeightRoot")->transform_;
	main_camera_ = GameObject::Find("MainCamera")->GetComponent<Camera>();
}

void PlayerCameraController::Infomation()
{
	ImGui::DragFloat2("Height Limit", &height_limit_min_max.x);
	ImGui::DragFloat("Rotation Speed", &rotation_speed_);
	ImGui::DragFloat("Height Speed", &height_speed_);
}

void PlayerCameraController::UpdateCameraSpin()
{
	// カメラ回転（左右）
	Vector3 y_rot{ Vector3(0,Input::GetAxis(GamePadAxis::AXIS_RX) * rotation_speed_, 0) };
	Vector3 r_result{ rotation_root_->quaternion->EulerAngles() + y_rot };
	Quaternion quq{ Quaternion::Euler(r_result) };
	rotation_root_->quaternion = quq;

	// カメラ移動（上下）
	Vector3 y_height{ Vector3(0,-Input::GetAxis(GamePadAxis::AXIS_RY) * height_speed_,0) };
	Vector3 h_result{ height_root_->transform_->localPosition + y_height };
	if (h_result.y > height_limit_min_max.y) h_result.y = height_limit_min_max.y;
	else if (h_result.y <= height_limit_min_max.x) h_result.y = height_limit_min_max.x;
	height_root_->transform_->localPosition = h_result;

	
}

void PlayerCameraController::UpdateCameraLook(const Transform &player)
{
	Vector3 camera_marker{ player.position };
	camera_marker.y += 1.5f;
	camera_marker += main_camera_->transform_->GetRight() * 0.5f;
	Vector3 cam_look{ (camera_marker - main_camera_->transform_->position) };
	Vector3::Normalize(cam_look);
	main_camera_->transform_->forward = cam_look;
}

void PlayerCameraController::FixedUpdateCameraPosition(const Transform &player)
{
	transform_->position = static_cast<Vector3>(player.position);
}

void PlayerCameraController::DashCameraFov()
{
	// カメラズーム
	main_camera_->fieldOfView = Ease(In, Quad, 0.6f, main_camera_->fieldOfView, 120.0f);
}

void PlayerCameraController::DefaultCameraFov()
{
	main_camera_->fieldOfView = Ease(In, Quad, 0.6f, main_camera_->fieldOfView, k_defo_fov_);
}
