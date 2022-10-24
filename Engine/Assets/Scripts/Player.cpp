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
	state_update_[PlayerState::DODGE] = &PlayerController::Dodge;
	state_update_[PlayerState::DASH] = &PlayerController::Dash;
	state_update_[PlayerState::MELEE_ATTACK_1] = &PlayerController::MeleeAttack1;

	//state_fixedupdate_[static_cast<int>(PlayerState::WALK)] = &PlayerController::WalkFixed;
}

void PlayerController::OnCollisionEnter(Collision &collision)
{
	//if (collision.gameObject.r_->name.r_ == "Floor" && collision.collider.r_->game_object_->name.r_ == "Player")
	//{
	//	transform_->position = {
	//		transform_->position.r_.x,
	//		collision.gameObject.r_->transform_->position.r_.y /*+ dynamic_cast<SphereCollider*>(collision.collider.r_)->radius*/,
	//		transform_->position.r_.z };
	//	if (player_state_ == PlayerState::JUMP)
	//	{
	//		player_state_ = PlayerState::IDOLE;
	//	}
	//	can_jump_ = true;

	//}
}

void PlayerController::OnCollisionStay(Collision &collision)
{
	if(collision.gameObject.r_->name.r_ == "Floor" && collision.collider.r_->game_object_->name.r_ == "Player")
	{
		//Vector3 penalty = collision.gameObject.r_->transform_->position.r_ - transform_->position.r_;
		//transform_->position = {
		//	transform_->position.r_.x,
		//	transform_->position.r_.y + penalty.y,
		//	//collision.gameObject.r_->transform_->position.r_.y /*+ dynamic_cast<SphereCollider*>(collision.collider.r_)->radius*/,
		//	transform_->position.r_.z};
		if(player_state_ == PlayerState::JUMP)
		{
			player_state_ = PlayerState::IDOLE;
		}
		can_jump_ = true;

	}
}

void PlayerController::Start()
{

	transform_->position = { 0.0f,0.0f,-50.0f };

	transform_->scale = { 0.4f,0.4f,0.4f };

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

	//// ��]����
	//Vector3 target_position;
	//if (
	//	regidbody_->velocity.r_.Magnitude() != 0.0f&&
	//	regidbody_->velocity.r_.x !=0.0f &&
	//	regidbody_->velocity.r_.z !=0.0f
	//	)
	//{
	//	target_position = Vector3::Scale(regidbody_->velocity.r_.Normalized(), Vector3(1.0f, 0.0f, 1.0f)) + transform_->position;
	//	
	//}
	//else
	//{
	//	target_position = transform_->position + transform_->GetFront();
	//}
	//transform_->LookAt(target_position);

	//// �L�[���͂ł̈ړ�����
	//if (Input::GetKeyPress(DIK_W))
	//{
	//	if (Input::GetKeyPress(DIK_LSHIFT))
	//	{

	//		regidbody_->AddForce(transform_->GetFront());
	//	}
	//	else
	//	{
	//		regidbody_->AddForce(transform_->GetFront());
	//	}

	//}
	//if (Input::GetKeyPress(DIK_S)) {
	//	// �ړ�
	//	regidbody_->AddForce(-transform_->GetFront());
	//}
	//if (Input::GetKeyPress(DIK_D))
	//{
	//	// �ړ�
	//	regidbody_->AddForce(transform_->GetRight());
	//}
	//if (Input::GetKeyPress(DIK_A))
	//{
	//	// �ړ�
	//	regidbody_->AddForce(-transform_->GetRight());
	//}


	//regidbody_->AddForce(transform_->GetFront() *
	//	-Input::GetAxis(GamePadAxis::AXIS_LY));
	//regidbody_->AddForce(transform_->GetRight() *
	//	Input::GetAxis(GamePadAxis::AXIS_LX));

}

void PlayerController::Update()
{

	input_horizontal_ = Input::GetAxis(GamePadAxis::AXIS_LX);
	input_vertical_ = -Input::GetAxis(GamePadAxis::AXIS_LY);

	(this->*state_update_[player_state_])(false);
	// �Y������X�e�[�g�Ăяo��

	//if(Input::GetAxis(GamePadAxis::AXIS_LZ) < 0.0f)
	//{
	//	if(is_dash_ == false)
	//	{
	//		Vector3 first_dush;;
	//		if (input_horizontal_ != 0.0f||
	//			input_vertical_ != 0.0f
	//			)
	//		{
	//			Vector3 camera_forward = Vector3::Scale(Camera::main.r_->transform_->GetFront(), Vector3(1.0f, 0.0f, 1.0f)).Normalized();

	//			first_dush = camera_forward * input_vertical_ + Camera::main.r_->transform_->GetRight() * input_horizontal_;
	//		}
	//		else
	//		{
	//			first_dush = -Vector3::Scale(transform_->GetFront(), Vector3(1.0f, 0.0f, 1.0f)).Normalized();
	//		}
	//		regidbody_->AddForce(first_dush * 100.0f);
	//	}
	//	is_dash_ = true;

	//}
	//else
	//{
	//	is_dash_ = false;
	//}
	//if (!(Input::GetKeyPress(DIK_W) ||
	//	Input::GetKeyPress(DIK_A) ||
	//	Input::GetKeyPress(DIK_S) ||
	//	Input::GetKeyPress(DIK_D)
	//	)) {
	//	if (animation_state_ != AnimationState::SLASH ||
	//		!game_object->IsPlayAnimation())
	//	{
	//		animation_state_ = AnimationState::IDOLE;
	//		game_object->PlayAnimation(static_cast<int>(animation_state_));

	//	}

	//}
	//else {
	//	// �L�[���͂ł̈ړ�����
	//	if (Input::GetKeyPress(DIK_W)) {
	//		// �ړ�
	//		if (Input::GetKeyPress(DIK_LSHIFT))
	//		{
	//			animation_state_ = AnimationState::RUN_FRONT;
	//		}
	//		else
	//		{
	//			animation_state_ = AnimationState::WALK_FRONT;
	//		}
	//		game_object->PlayAnimation(static_cast<int>(animation_state_));
	//	}
	//	else if (Input::GetKeyPress(DIK_S)) {
	//		// �ړ�
	//		animation_state_ = AnimationState::WALK_BACK;
	//		game_object->PlayAnimation(static_cast<int>(animation_state_));
	//	}
	//	else {
	//		if (Input::GetKeyPress(DIK_D)) {

	//			// �ړ�
	//			animation_state_ = AnimationState::WALK_RIGHT;
	//			game_object->PlayAnimation(static_cast<int>(animation_state_));
	//		}
	//		if (Input::GetKeyPress(DIK_A)) {
	//			// �ړ�
	//			animation_state_ = AnimationState::WALK_LEFT;
	//			game_object->PlayAnimation(static_cast<int>(animation_state_));
	//		}
	//	}
	//}


	//i/*f (Input::GetKeyPress(DIK_Q))
	//{
	//	spin -= 2;
	//	XMStoreFloat4(&transform_->localQuaternion, XMQuaternionRotationRollPitchYaw(0, spin * Mathf::deg_to_rad, 0));
	//}
	//if (Input::GetKeyPress(DIK_E))
	//{
	//	spin += 2;
	//	XMStoreFloat4(&transform_->localQuaternion, XMQuaternionRotationRollPitchYaw(0, spin * Mathf::deg_to_rad, 0));
	//}*/

	//if (/*Input::GetKeyPressTrigger(DIK_SPACE) ||*/
	//	Input::GetButtonPressTrigger(GamePadButton::INPUT_X)) {
	//	animation_state_ = AnimationState::SLASH;
	//	game_object->PlayAnimation(static_cast<int>(animation_state_), false);
	//}
	//// �W�����v
	//if (Input::GetButtonPressTrigger(GamePadButton::INPUT_A)) {
	//	animation_state_ = AnimationState::Jump;
	//	game_object->PlayAnimation(static_cast<int>(animation_state_), false);
	//}

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
		/*if (
			Input::GetButtonPressTrigger(GamePadButton::INPUT_A) &&
			can_jump_
			)
		{
			player_state_ = PlayerState::JUMP;
			return;
		}*/
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

		Vector3 move_forward = (camera_forward * input_vertical_ + Camera::main.r_->transform_->GetRight() * input_horizontal_).Normalized() * 25.0f;
		
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

		model_data_->PlayAnimation(static_cast<int>(AnimationState::RUN_FRONT));
	}
	else
	{
		// �J�����̐��ʃx�N�g��
		Vector3 camera_forward = Vector3::Scale(Camera::main.r_->transform_->GetFront(), Vector3(1.0f, 0.0f, 1.0f)).Normalized();

		Vector3 move_forward = (camera_forward * input_vertical_ + Camera::main.r_->transform_->GetRight() * input_horizontal_).Normalized() * 25.0f;

		if (input_horizontal_ != 0.0f ||
			input_vertical_ != 0.0f)
		{
			regidbody_->AddForce(move_forward * 1.8f, ForceMode::Acceleration);
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
			regidbody_->AddForce(Vector3::up * 200, ForceMode::Impulse);
			can_jump_ = false;
		}
	}
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

