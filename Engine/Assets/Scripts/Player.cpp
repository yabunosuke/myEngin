#include "Player.h"

#include "Input.h"
#include "Object3dComponent.h"
#include "Object/GameObject/GameObject.h"
#include "Object/Component/Camera.h"
#include "Object/Component/Collider/SphereCollider/SphereCollider.h"

class Object3dComponent;



PlayerController::PlayerController():
MonoBehaviour("PlayerController")
{
	state_update_[PlayerState::IDOLE] = &PlayerController::Idole;
	state_update_[PlayerState::WALK] = &PlayerController::Walk;
	state_update_[PlayerState::JUMP] = &PlayerController::Jump;
	state_update_[PlayerState::JUMP_DROP] = &PlayerController::JumpDrop;
	state_update_[PlayerState::DODGE] = &PlayerController::Dodge;
	state_update_[PlayerState::DASH] = &PlayerController::Dash;
	state_update_[PlayerState::MELEE_ATTACK_1] = &PlayerController::MeleeAttack1;

	//state_fixedupdate_[static_cast<int>(PlayerState::WALK)] = &PlayerController::WalkFixed;
}

void PlayerController::OnCollisionEnter(Collision &collision)
{
	if (Vector3::Dot(collision.contactPoint->normal,{0,1,0}) >= 0.8f)
	{
		if (player_state_ == PlayerState::JUMP)
		{
			player_state_ = PlayerState::IDOLE;
		}
		can_jump_ = true;

	}
}

void PlayerController::OnCollisionStay(Collision &collision)
{
	if (Vector3::Dot(collision.contactPoint->normal,Vector3::up) >=0.8f)
	{
		if(player_state_ == PlayerState::JUMP_DROP)
		{
			player_state_ = PlayerState::IDOLE;
		}
		can_jump_ = true;
	}
	//{
	//	//Vector3 penalty = collision.gameObject.r_->transform_->position.r_ - transform_->position.r_;
	//	//transform_->position = {
	//	//	transform_->position.r_.x,
	//	//	transform_->position.r_.y + penalty.y,
	//	//	//collision.gameObject.r_->transform_->position.r_.y /*+ dynamic_cast<SphereCollider*>(collision.collider.r_)->radius*/,
	//	//	transform_->position.r_.z};
	//	//if(player_state_ == PlayerState::JUMP)
	//	{
	//		player_state_ = PlayerState::IDOLE;
	//	}
	//	can_jump_ = true;

	//}
}

void PlayerController::Start()
{

	transform_->position = { 0.0f,10.0f,0.0f };


	// ���W�b�h
	regidbody_ = 
		game_object_->GetComponent<Rigidbody>();
	//regidbody_->useGravity = true;
	model_data_ = 
		game_object_->GetComponent<Object3dComponent>()->GetObjectData();
}

void PlayerController::FixedUpdate()
{
	(this->*state_update_[player_state_])(true);

}

void PlayerController::Update()
{

	input_horizontal_ = Input::GetAxis(GamePadAxis::AXIS_LX);
	input_vertical_ = -Input::GetAxis(GamePadAxis::AXIS_LY);

	(this->*state_update_[player_state_])(false);
	
}

void PlayerController::Idole(bool is_fixed)
{
	if(!is_fixed)
	{
		// �A�C�h���A�j���[�V�����Đ�
		model_data_->PlayAnimation(static_cast<int>(AnimationState::IDOLE), true);

		// ���s
		if (
			input_horizontal_ != 0.0f ||
			input_vertical_ != 0.0f
			)
		{
			player_state_ = PlayerState::WALK;
			return;
		}

		// �U��
		if (
			Input::GetButtonPressTrigger(GamePadButton::INPUT_X)
			)
		{
			player_state_ = PlayerState::MELEE_ATTACK_1;
			return;
		}

		// �W�����v
		if (
			Input::GetButtonPressTrigger(GamePadButton::INPUT_A) &&
			can_jump_
			)
		{
			player_state_ = PlayerState::JUMP;
			return;
		}
	}
	else
	{
	}

	return;
}

void PlayerController::Walk(bool is_fixed)
{
	if(!is_fixed)
	{
		// ���͂������Ȃ�����Idole�ɖ߂�
		if (
			input_horizontal_ == 0.0f &&
			input_vertical_ == 0.0f
			)
		{
			player_state_ = PlayerState::IDOLE;
			return;
		}

		// �U��
		if (
			Input::GetButtonPressTrigger(GamePadButton::INPUT_X)
			)
		{
			player_state_ = PlayerState::MELEE_ATTACK_1;
			return;
		}
		
		// ���
		if (
			Input::GetAxis(GamePadAxis::AXIS_LZ) < 0.0f
			)
		{
			player_state_ = PlayerState::DODGE;

		}

		// �W�����v
		if (
			Input::GetButtonPressTrigger(GamePadButton::INPUT_A) &&
			can_jump_
			)
		{
			player_state_ = PlayerState::JUMP;
			return;
		}

		// ���ʂֈړ����Ă���ꍇ
		//if (input_vertical_ >= 0.0f)
		{
			model_data_->PlayAnimation(static_cast<int>(AnimationState::WALK_FRONT));
		}
		/*else if (input_horizontal_ > 0.0f)
		{
			model_data_->PlayAnimation(static_cast<int>(AnimationState::WALK_RIGHT));
		}
		else if (input_horizontal_ < 0.0f)
		{
			model_data_->PlayAnimation(static_cast<int>(AnimationState::WALK_LEFT));
		}*/
	}
	else
	{
		// �J�����̐��ʃx�N�g��
		Vector3 camera_forward = Vector3::Scale(Camera::main.r_->transform_->GetFront(), Vector3(1.0f, 0.0f, 1.0f)).Normalized();

		Vector3 move_forward = (camera_forward * input_vertical_ + Camera::main.r_->transform_->GetRight() * input_horizontal_).Normalized();
		
		if (input_horizontal_ != 0.0f ||
			input_vertical_ != 0.0f)
		{
			regidbody_->AddForce(move_forward, ForceMode::Acceleration);
		}


		// ��]����
		Vector3 target_position;
		if (
			regidbody_->velocity.r_.Magnitude() != 0.0f &&
			regidbody_->velocity.r_.x != 0.0f &&
			regidbody_->velocity.r_.z != 0.0f
			)
		{
			target_position = Vector3::Scale(regidbody_->velocity.r_.Normalized(), Vector3(1.0f, 0.0f, 1.0f)) + transform_->position;
		}
		else
		{
			target_position = transform_->position + transform_->GetFront();
		}
		transform_->LookAt(target_position);
	}
}

void PlayerController::Dash(bool is_fixed)
{
	if (!is_fixed)
	{
		// �_�b�V���L�[���������ꂽ�����
		if (
			Input::GetAxis(GamePadAxis::AXIS_LZ) == 0.0f &&
			( input_horizontal_ != 0.0f || input_vertical_ != 0.0f )
			)
		{
			player_state_ = PlayerState::WALK;
			return;
		}

		// ���͂������Ȃ�����Idole�ɖ߂�
		if (input_horizontal_ == 0.0f &&
			input_vertical_ == 0.0f	)
		{
			player_state_ = PlayerState::IDOLE;
			return;
		}

		// �U��
		if (
			Input::GetButtonPressTrigger(GamePadButton::INPUT_X)
			)
		{
			player_state_ = PlayerState::MELEE_ATTACK_1;
			return;
		}

		// �W�����v
		if (
			Input::GetButtonPressTrigger(GamePadButton::INPUT_A) &&
			can_jump_
			)
		{
			player_state_ = PlayerState::JUMP;
			return;
		}

		model_data_->PlayAnimation(static_cast<int>(AnimationState::RUN_FRONT));
	}
	else
	{
		// �J�����̐��ʃx�N�g��
		Vector3 camera_forward = Vector3::Scale(Camera::main.r_->transform_->GetFront(), Vector3(1.0f, 0.0f, 1.0f)).Normalized();

		Vector3 move_forward = (camera_forward * input_vertical_ + Camera::main.r_->transform_->GetRight() * input_horizontal_).Normalized() * 2.0f;

		if (input_horizontal_ != 0.0f ||
			input_vertical_ != 0.0f)
		{
			regidbody_->AddForce(move_forward, ForceMode::Acceleration);
		}


		// ��]����
		Vector3 target_position;
		if (
			regidbody_->velocity.r_.Magnitude() != 0.0f &&
			regidbody_->velocity.r_.x != 0.0f &&
			regidbody_->velocity.r_.z != 0.0f
			)
		{
			target_position = Vector3::Scale(regidbody_->velocity.r_.Normalized(), Vector3(1.0f, 0.0f, 1.0f)) + transform_->position;

		}
		else
		{
			target_position = transform_->position + transform_->GetFront();
		}
		transform_->LookAt(target_position);
	}
}

void PlayerController::Jump(bool is_fixed)
{
	if(!is_fixed)
	{
		if (can_jump_)
		{
			regidbody_->AddForce(Vector3::up * 2.0f, ForceMode::Impulse);
			can_jump_ = false;
			model_data_->PlayAnimation(static_cast<int>(AnimationState::Jump),false);
		}
	}
	else
	{
		// �J�����̐��ʃx�N�g��
		Vector3 camera_forward = Vector3::Scale(Camera::main.r_->transform_->GetFront(), Vector3(1.0f, 0.0f, 1.0f)).Normalized();

		Vector3 move_forward = (camera_forward * input_vertical_ + Camera::main.r_->transform_->GetRight() * input_horizontal_).Normalized() * 1.5f;

		if (input_horizontal_ != 0.0f ||
			input_vertical_ != 0.0f)
		{
			regidbody_->AddForce(move_forward, ForceMode::Acceleration);
		}


		// ��]����
		Vector3 target_position;
		if (
			regidbody_->velocity.r_.Magnitude() != 0.0f &&
			regidbody_->velocity.r_.x != 0.0f &&
			regidbody_->velocity.r_.z != 0.0f
			)
		{
			target_position = Vector3::Scale(regidbody_->velocity.r_.Normalized(), Vector3(1.0f, 0.0f, 1.0f)) + transform_->position;

		}
		else
		{
			target_position = transform_->position + transform_->GetFront();
		}
		transform_->LookAt(target_position);

		if (regidbody_->velocity->y < 0.0f)
		{
			player_state_ = PlayerState::JUMP_DROP;
		}
	}
}

void PlayerController::JumpDrop(bool is_fixed)
{
}

void PlayerController::Dodge(bool is_fixed)
{
	if (!is_fixed)
	{
		// �X�e�B�b�N���͂�����΃_�b�V��
		if (
			Input::GetAxis(GamePadAxis::AXIS_LZ) < 0.0f &&
			(input_horizontal_ != 0.0f || input_vertical_ != 0.0f)
			)
		{
			player_state_ = PlayerState::DASH;
			return;
		}
	}
	else
	{

	}
}

void PlayerController::MeleeAttack1(bool is_fixed)
{
	if (!is_fixed)
	{
		// �A�j���[�V�������I�������A�C�h���ɖ߂�
		if (!model_data_->IsPlayAnimation())
		{
			player_state_ = PlayerState::IDOLE;
			return;
		}

		// �A�j���[�V����
		model_data_->PlayAnimation(static_cast<int>(AnimationState::SLASH), false);
	}
}

