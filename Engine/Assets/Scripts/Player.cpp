#include "Player.h"

#include "Input.h"
#include "Object3dComponent.h"
#include "Math/Easing.h"
#include "Object/GameObject/GameObject.h"
#include "Object/Component/Camera.h"
#include "Object/Component/Collider/SphereCollider/SphereCollider.h"
#include <Time/Time.h>

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
	state_update_[PlayerState::MELEE_ATTACK_2] = &PlayerController::MeleeAttack2;

	//state_fixedupdate_[static_cast<int>(PlayerState::WALK)] = &PlayerController::WalkFixed;
}

void PlayerController::OnCollisionEnter(Collision &collision)
{
	if (Vector3::Dot(collision.contactPoint->normal,{0,1,0}) >= 0.8f)
	{

	}
}

void PlayerController::OnCollisionStay(Collision &collision)
{
	if (Vector3::Dot(collision.contactPoint->normal,Vector3::up) >=0.8f)
	{
		if(playerState == PlayerState::JUMP_DROP)
		{
			playerState = PlayerState::IDOLE;
			can_jump_ = true;
		}
	}
}

void PlayerController::Awake()
{
	transform_->position = { -3.0f,0.0f,-3.0f };
	transform_->quaternion = Quaternion::Euler(0, 45.0f * Mathf::deg_to_rad, 0);
}

void PlayerController::Start()
{
	// リジッド
	rigidbody_ = 
		game_object_->GetComponent<Rigidbody>();
	transform_->scale = { 0.5f,0.5f,0.5f };
	model_data_ = 
		game_object_->GetComponent<Object3dComponent>()->GetObjectData();
	camera_controller_ =
		GameObject::Find("CameraRoot")->GetComponent<PlayerCameraController>();
}

void PlayerController::FixedUpdate()
{
	(this->*state_update_[playerState])(true);
	// 移動処理
	Vector3 move = input_stick_r_ * move_speed;
	Vector3 camera_move = Camera::main.r_->transform_->quaternion * move;
	camera_move.y = 0;
	rigidbody_->AddForce(camera_move - Vector3::Scale({1,0,1}, rigidbody_->velocity), ForceMode::VelocityChange);
	// 回転処理
	if(input_stick_r_.x != 0.0f || input_stick_r_.z != 0.0f)
	{
		Vector3 dir = rigidbody_->velocity->Normalized();
		dir.y = 0.0f;
		transform_->LookAt(transform_->position + dir);
	}

	// カメラ移動処理
	camera_controller_->FixedUpdateCameraPosition(*transform_);
	if(player_state_ != PlayerState::DASH)
	{
		camera_controller_->DefaultCameraFov();
	}
}

void PlayerController::Update()
{

	input_stick_r_ = 
	{
		Input::GetAxis(GamePadAxis::AXIS_LX),
		0,
		-Input::GetAxis(GamePadAxis::AXIS_LY)
	};

	(this->*state_update_[playerState])(false);

	// カメラの回転処理
	camera_controller_->UpdateCameraLook(*transform_);

	camera_controller_->UpdateCameraSpin();
}

void PlayerController::Idole(bool is_fixed)
{
	if(!is_fixed)
	{
		// アイドルアニメーション再生
		model_data_->PlayAnimation(static_cast<int>(AnimationState::Idol), true);

		// 歩行
		if (input_stick_r_.Magnitude() != 0.0f)
		{
			playerState = PlayerState::WALK;
			return;
		}

		// 攻撃
		if (
			Input::GetButtonPressTrigger(GamePadButton::INPUT_X)
			)
		{
			playerState = PlayerState::MELEE_ATTACK_1;
			return;
		}

		// ジャンプ
		if (
			Input::GetButtonPressTrigger(GamePadButton::INPUT_A) &&
			can_jump_
			)
		{
			playerState = PlayerState::JUMP;
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
		move_speed = 2.0f;
		// 入力が無くなったらIdoleに戻る
		if (input_stick_r_.Magnitude() == 0.0f)
		{
			playerState = PlayerState::IDOLE;
			return;
		}

		// 攻撃
		if (
			Input::GetButtonPressTrigger(GamePadButton::INPUT_X)
			)
		{
			playerState = PlayerState::MELEE_ATTACK_1;
			return;
		}
		
		// 回避
		if (
			Input::GetAxis(GamePadAxis::AXIS_LZ) < 0.0f
			)
		{
			playerState = PlayerState::DODGE;

		}

		// ジャンプ
		if (
			Input::GetButtonPressTrigger(GamePadButton::INPUT_A) &&
			can_jump_
			)
		{
			playerState = PlayerState::JUMP;
			return;
		}
		
		model_data_->PlayAnimation(static_cast<int>(AnimationState::Walk));
		
	}
	else
	{
		//if ((input_vertical_ != 0.0f) || (input_horizontal_ != 0.0f))
		{
			// カメラの正面ベクトル
			

			// 回転処理
			//transform_->LookAt(move_forward.Normalized() + transform_->position);
		}
	}
}

void PlayerController::Dash(bool is_fixed)
{
	if (!is_fixed)
	{
		move_speed = 5.0f;

		// ダッシュキーだけ離されたら歩く
		if (
			Input::GetAxis(GamePadAxis::AXIS_LZ) == 0.0f &&
			input_stick_r_.Magnitude() != 0.0f)
		{
			playerState = PlayerState::WALK;
			return;
		}

		// 入力が無くなったらIdoleに戻る
		if (input_stick_r_.Magnitude() == 0.0f)
		{
			playerState = PlayerState::IDOLE;
			return;
		}

		// 攻撃
		if (
			Input::GetButtonPressTrigger(GamePadButton::INPUT_X)
			)
		{
			playerState = PlayerState::MELEE_ATTACK_1;
			return;
		}

		// ジャンプ
		if (
			Input::GetButtonPressTrigger(GamePadButton::INPUT_A) &&
			can_jump_
			)
		{
			playerState = PlayerState::JUMP;
			return;
		}

		model_data_->PlayAnimation(static_cast<int>(AnimationState::Run));
	}
	else
	{
		camera_controller_->DashCameraFov();
	}
}

void PlayerController::Jump(bool is_fixed)
{
	if(!is_fixed)
	{
		if (can_jump_)
		{
			rigidbody_->AddForce(Vector3::up * 5.0f, ForceMode::Impulse);
			can_jump_ = false;
			model_data_->PlayAnimation(static_cast<int>(AnimationState::Jump),false);
		}
	}
	else
	{
		if(input_stick_r_.Magnitude() != 0.0f)
		{
			// カメラの正面ベクトル
			//Vector3 camera_forward = Vector3::Scale(Camera::main.r_->transform_->GetFront(), Vector3(1.0f, 0.0f, 1.0f)).Normalized();

			//Vector3 move_forward = (camera_forward * input_vertical_ + Camera::main.r_->transform_->GetRight() * input_horizontal_).Normalized() * 1.5f;

			//if (input_horizontal_ != 0.0f ||
			//	input_vertical_ != 0.0f)
			//{
			//	rigidbody_->AddForce(move_forward, ForceMode::Acceleration);
			//}

			//// 回転処理
			//transform_->LookAt(move_forward.Normalized() + transform_->position);
		}

		if (rigidbody_->velocity->y <= Mathf::epsilon)
		{
			playerState = PlayerState::JUMP_DROP;
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
		// スティック入力があればダッシュ
		if (
			Input::GetAxis(GamePadAxis::AXIS_LZ) < 0.0f &&
			(input_stick_r_.Magnitude() != 0.0f)
			)
		{
			playerState = PlayerState::DASH;
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
		static float timer = 0;
		if (timer >= 0.2f)
		{
			timer = 0.2f;

			if (Input::GetButtonPressTrigger(GamePadButton::INPUT_X))
			{
				timer = 0.0f;
				is_nextattack_ = true;
			}
		}
		else
		{
			timer += Time::GetInstance()->time;
		}

		

		// アニメーション
		model_data_->PlayAnimation(static_cast<int>(AnimationState::Attack1), false);
	}
	else
	{
		// アニメーションが終わったらアイドルに戻る
		if (!model_data_->IsPlayAnimation())
		{
			if (is_nextattack_)
			{
				is_nextattack_ = false;
				playerState = PlayerState::MELEE_ATTACK_2;
			}
			else
			{
				playerState = PlayerState::IDOLE;
			}
			return;
		}
	}
}

void PlayerController::MeleeAttack2(bool is_fixed)
{
	if (!is_fixed)
	{
		// アニメーション
		model_data_->PlayAnimation(static_cast<int>(AnimationState::Attack2), false);
	}
	else
	{
		// アニメーションが終わったらアイドルに戻る
		if (!model_data_->IsPlayAnimation())
		{
			playerState = PlayerState::IDOLE;
			return;
		}
	}
}

