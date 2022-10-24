#include "Player.h"

#include "Input.h"
#include "Object3dComponent.h"
#include "Object/GameObject/GameObject.h"
#include "Object/Component/Camera.h"

class Object3dComponent;



PlayerController::PlayerController():
MonoBehaviour("PlayerController")
{
	state_update_[PlayerState::IDOLE] = &PlayerController::Idole;
	state_update_[PlayerState::WALK] = &PlayerController::Walk;
	state_update_[PlayerState::JUMP] = &PlayerController::Jump;
	state_update_[PlayerState::MELEE_ATTACK_1] = &PlayerController::MeleeAttack1;

	//state_fixedupdate_[static_cast<int>(PlayerState::WALK)] = &PlayerController::WalkFixed;
}

void PlayerController::OnCollisionEnter(Collision &collision)
{

}

void PlayerController::Start()
{

	transform_->position = { 0.0f,0.0f,-50.0f };

	transform_->scale = { 0.4f,0.4f,0.4f };

	// リジッド
	regidbody_ = 
		game_object_->GetComponent<Rigidbody>();
	model_data_ = 
		game_object_->GetComponent<Object3dComponent>()->GetObjectData();
}

void PlayerController::FixedUpdate()
{
	(this->*state_update_[player_state_])(true);

	//// 回転処理
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

	//// キー入力での移動処理
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
	//	// 移動
	//	regidbody_->AddForce(-transform_->GetFront());
	//}
	//if (Input::GetKeyPress(DIK_D))
	//{
	//	// 移動
	//	regidbody_->AddForce(transform_->GetRight());
	//}
	//if (Input::GetKeyPress(DIK_A))
	//{
	//	// 移動
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
	// 該当するステート呼び出し

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
	//	// キー入力での移動処理
	//	if (Input::GetKeyPress(DIK_W)) {
	//		// 移動
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
	//		// 移動
	//		animation_state_ = AnimationState::WALK_BACK;
	//		game_object->PlayAnimation(static_cast<int>(animation_state_));
	//	}
	//	else {
	//		if (Input::GetKeyPress(DIK_D)) {

	//			// 移動
	//			animation_state_ = AnimationState::WALK_RIGHT;
	//			game_object->PlayAnimation(static_cast<int>(animation_state_));
	//		}
	//		if (Input::GetKeyPress(DIK_A)) {
	//			// 移動
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
	//// ジャンプ
	//if (Input::GetButtonPressTrigger(GamePadButton::INPUT_A)) {
	//	animation_state_ = AnimationState::Jump;
	//	game_object->PlayAnimation(static_cast<int>(animation_state_), false);
	//}

}

void PlayerController::Idole(bool is_fixed)
{
	if(!is_fixed)
	{
		// アイドルアニメーション再生
		model_data_->PlayAnimation(static_cast<int>(AnimationState::IDOLE), true);

		// 歩行
		if (
			input_horizontal_ != 0.0f ||
			input_vertical_ != 0.0f
			)
		{
			player_state_ = PlayerState::WALK;
			return;
		}

		// 攻撃
		if (
			Input::GetButtonPressTrigger(GamePadButton::INPUT_X)
			)
		{
			player_state_ = PlayerState::MELEE_ATTACK_1;
			return;
		}

		// ジャンプ
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
		// 入力が無くなったらIdoleに戻る
		if (
			input_horizontal_ == 0.0f &&
			input_vertical_ == 0.0f
			)
		{
			player_state_ = PlayerState::IDOLE;
			return;
		}

		// 攻撃
		if (
			Input::GetButtonPressTrigger(GamePadButton::INPUT_X)
			)
		{
			player_state_ = PlayerState::MELEE_ATTACK_1;
			return;
		}
		
		// ダッシュ
		if (
			Input::GetAxis(GamePadAxis::AXIS_LZ) < 0.0f
			)
		{
			player_state_ = PlayerState::DODGE;

		}


		// 正面へ移動している場合
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
		// カメラの正面ベクトル
		Vector3 camera_forward = Vector3::Scale(Camera::main.r_->transform_->GetFront(), Vector3(1.0f, 0.0f, 1.0f)).Normalized();

		Vector3 move_forward = (camera_forward * input_vertical_ + Camera::main.r_->transform_->GetRight() * input_horizontal_).Normalized() * 25.0f;
		
		if (input_horizontal_ != 0.0f ||
			input_vertical_ != 0.0f)
		{
			regidbody_->AddForce(move_forward, ForceMode::Acceleration);
		}


		// 回転処理
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
			regidbody_->AddForce(Vector3::up * 10, ForceMode::Impulse);
			can_jump_ = false;
		}
	}
}

void PlayerController::DodgeBack(bool is_fixed)
{
	if (!is_fixed)
	{


	}
	else
	{

	}
}

void PlayerController::MeleeAttack1(bool is_fixed)
{
	if (!is_fixed)
	{
		// アニメーションが終わったらアイドルに戻る
		if (!model_data_->IsPlayAnimation())
		{
			player_state_ = PlayerState::IDOLE;
			return;
		}

		// アニメーション
		model_data_->PlayAnimation(static_cast<int>(AnimationState::SLASH), false);
	}
}

