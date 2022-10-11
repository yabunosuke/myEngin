#include "CameraController.h"
#include "Input.h"
#include "../Engine/Time/Time.h"

CameraController::CameraController(GameObject *target_object):
	MonoBehaviour("CameraController"),
	target_object_(target_object)
{
}

void CameraController::Start()
{
	camera_ = game_object_->GetComponent<Camera>();
	target_position_ = target_object_->transform_->position.r_;
	camera_->target = &target_position_;
}

void CameraController::FixedUpdate()
{
	if (target_object_ == nullptr) return;

	Vector3 self_position = transform_->position;

	// ターゲット座標更新
	target_position_ = target_object_->transform_->position.r_;

	float x = Input::GetAxis(GamePadAxis::AXIS_RX);
	float y = Input::GetAxis(GamePadAxis::AXIS_RY);

	// 現在位置とターゲットが重なった場合の処理
	if (self_position == target_position_)
	{
		Vector3 rotate_axis_normal = rotate_axis_.Normalized();
		Vector3 dummy_direct_vector = Vector3::forward;
		if (fabsf(rotate_axis_normal.y) < 0.5f)
		{
			dummy_direct_vector = Vector3::up;
		}

		Vector3 direct_vector = Vector3::Cross(rotate_axis_, dummy_direct_vector).Normalized();
		self_position = direct_vector * radius_distance_;
	}

	Vector3 diff_vector = self_position - target_position_;
	float diff_magnitude = diff_vector.Magnitude();
	float dot = Vector3::Dot(diff_vector.Normalized(), rotate_axis_);

	self_position -= rotate_axis_.Normalized() * (diff_magnitude * dot);

	// 現在距離と半径距離の差分
	float diff_distance = Vector3::Distance(self_position, target_position_) - radius_distance_;

	// 指定半径になるように移動
	//transform_->position = Vector3::MoveTowards(self_position, target_position_, diff_distance);

	// ターゲットを中心に回転する
	if(x != 0.0f)
	{
		transform_->RotateAround(
			target_position_,
			Vector3::up,
			x
		);
	}

	if (y != 0.0f)
	{
		transform_->RotateAround(
			target_position_,
			Vector3::right,
			y
		);
	}
}

void CameraController::Update()
{
}