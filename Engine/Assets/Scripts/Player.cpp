#include "Player.h"

#include "Input.h"
#include "Object3dComponent.h"
#include "Object/GameObject/GameObject.h"
#include "Object/Component/Camera.h"

class Object3dComponent;

PlayerController::PlayerController():
MonoBehaviour("PlayerController")
{
}

void PlayerController::OnCollisionEnter(Collision &collision)
{

}

void PlayerController::Start()
{
	transform_->localPosition = { 0.0f,0.0f,-50.0f };
	transform_->localScale = { 0.4f,0.4f,0.4f };


	// リジッド
	regidbody_ = 
		game_object_->GetComponent<Rigidbody>();
	game_object = 
		game_object_->GetComponent<Object3dComponent>()->GetObjectData();
}

void PlayerController::FixedUpdate()
{
	Vector3 camera_forward = Vector3::Scale(Camera::main.r_->transform_->GetFront(), Vector3(1.0f,0.0f,1.0f)).Normalized();
	
	Vector3 move_forward = camera_forward * input_vertical_ + Camera::main.r_->transform_->GetRight() * input_horizontal_;
	regidbody_->AddForce(move_forward);

	// 回転処理
	Vector3 target_position;
	if (regidbody_->velocity_.Magnitude() != 0.0f)
	{
		target_position = Vector3::Scale(regidbody_->velocity_.Normalized(), Vector3(1.0f, 0.0f, 1.0f)) + transform_->position;
	}
	else
	{
		target_position = transform_->position + transform_->GetFront();
	}
	transform_->LookAt(target_position);

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
	static float spin = 0;

	input_horizontal_ = Input::GetAxis(GamePadAxis::AXIS_LX);
	input_vertical_ = -Input::GetAxis(GamePadAxis::AXIS_LY);


	if (!(Input::GetKeyPress(DIK_W) ||
		Input::GetKeyPress(DIK_A) ||
		Input::GetKeyPress(DIK_S) ||
		Input::GetKeyPress(DIK_D)
		)) {
		if (state != AnimationState::SLASH ||
			!game_object->IsPlayAnimation())
		{
			state = AnimationState::IDOLE;
			game_object->PlayAnimation(static_cast<int>(state));

		}

	}
	else {
		// キー入力での移動処理
		if (Input::GetKeyPress(DIK_W)) {
			// 移動
			if (Input::GetKeyPress(DIK_LSHIFT))
			{
				state = AnimationState::RUN_FRONT;
			}
			else
			{
				state = AnimationState::WALK_FRONT;
			}
			game_object->PlayAnimation(static_cast<int>(state));
		}
		else if (Input::GetKeyPress(DIK_S)) {
			// 移動
			state = AnimationState::WALK_BACK;
			game_object->PlayAnimation(static_cast<int>(state));
		}
		else {
			if (Input::GetKeyPress(DIK_D)) {

				// 移動
				state = AnimationState::WALK_RIGHT;
				game_object->PlayAnimation(static_cast<int>(state));
			}
			if (Input::GetKeyPress(DIK_A)) {
				// 移動
				state = AnimationState::WALK_LEFT;
				game_object->PlayAnimation(static_cast<int>(state));
			}
		}
	}


	if (Input::GetKeyPress(DIK_Q))
	{
		spin -= 2;
		XMStoreFloat4(&transform_->localQuaternion, XMQuaternionRotationRollPitchYaw(0, spin * Mathf::deg_to_rad, 0));
	}
	if (Input::GetKeyPress(DIK_E))
	{
		spin += 2;
		XMStoreFloat4(&transform_->localQuaternion, XMQuaternionRotationRollPitchYaw(0, spin * Mathf::deg_to_rad, 0));
	}

	if (Input::GetKeyPressTrigger(DIK_SPACE) ||
		Input::GetButtonPressTrigger(GamePadButton::INPUT_A)) {
		state = AnimationState::SLASH;
		game_object->PlayAnimation(static_cast<int>(state), false);
	}
}
