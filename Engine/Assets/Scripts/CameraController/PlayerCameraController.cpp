#include "PlayerCameraController.h"
#include "Input.h"
#include "Math/Easing.h"

PlayerCameraController::PlayerCameraController() :
	MonoBehaviour("PlayerCameraController")
{
}

void PlayerCameraController::Start()
{
	rotation_root_ = GameObject::FindObject("RotationRoot").lock()->transform;
	height_root_ = GameObject::FindObject("HeightRoot").lock()->transform;
	main_camera_ = GameObject::FindObject("MainCamera").lock()->GetComponent<Camera>().lock().get();
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
	Vector3 r_result{ rotation_root_.lock()->quaternion->EulerAngles() + y_rot};
	Quaternion quq{ Quaternion::Euler(r_result) };
	rotation_root_.lock()->quaternion = quq;

	// カメラ移動（上下）
	Vector3 y_height{ Vector3(0,-Input::GetAxis(GamePadAxis::AXIS_RY) * height_speed_,0) };
	Vector3 h_result{ height_root_.lock()->transform->lock()->localPosition + y_height };
	if (h_result.y > height_limit_min_max.y) h_result.y = height_limit_min_max.y;
	else if (h_result.y <= height_limit_min_max.x) h_result.y = height_limit_min_max.x;
	height_root_.lock()->transform->lock()->localPosition = h_result;

	
}

void PlayerCameraController::UpdateCameraLook(const std::weak_ptr<Transform> player)
{
	Vector3 camera_marker{ player.lock()->position};
	camera_marker.y += 1.5f;
	camera_marker += main_camera_->transform->lock()->GetRight() * 0.5f;
	Vector3 cam_look{ (camera_marker - main_camera_->transform->lock()->position)};
	Vector3::Normalize(cam_look);
	main_camera_->transform->lock()->forward = cam_look;
}

void PlayerCameraController::FixedUpdateCameraPosition(const std::weak_ptr<Transform> player)
{
	transform->lock()->position = static_cast<Vector3>(player.lock()->position);
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
