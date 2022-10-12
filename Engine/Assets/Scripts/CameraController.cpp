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
}

void CameraController::FixedUpdate()
{
	if (target_object_ == nullptr) return;

	// ���݈ʒu
	Vector3 self_position = transform_->position;
	// �^�[�Q�b�g���W�X�V
	target_position_ = target_object_->transform_->position.r_;


	Vector3 input
	{
		Input::GetAxis(GamePadAxis::AXIS_RX),
		Input::GetAxis(GamePadAxis::AXIS_RY),
		0.0f
	};
	
	// ���݈ʒu�ƃ^�[�Q�b�g���d�Ȃ����ꍇ�̏���
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
	//self_position -= Vector3::Scale(rotate_axis_.Normalized(),(diff_vector * dot));
	// ���݋����Ɣ��a�����̍���
	float diff_distance = Vector3::Distance(self_position, target_position_) - radius_distance_;

	// �w�蔼�a�ɂȂ�悤�Ɉړ�
	transform_->position =
		Vector3::MoveTowards(self_position, target_position_, diff_distance * 0.02f);

	// �^�[�Q�b�g�𒆐S�ɉ�]����
	if(input.x != 0.0f)
	{
		transform_->RotateAround(
			target_position_,
			Vector3::up,
			input.x
		);
	}

	if (input.y != 0.0f)
	{
		Vector3 horizontal;
		if(input.y > 0.0f)
		{
			horizontal = Vector3::Cross(diff_vector.Normalized(), input.Normalized()).Normalized();

		}
		else
		{
			horizontal = Vector3::Cross(input.Normalized(), diff_vector.Normalized()).Normalized();
		}

		transform_->RotateAround(
			target_position_,
			horizontal.Normalized(),
			input.y
		);
	}
}

void CameraController::Update()
{
}