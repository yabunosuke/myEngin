#include "Player.h"

#include "Input.h"
#include "Object3dComponent.h"
#include "Object/GameObject/GameObject.h"

class Object3dComponent;

Player::Player():
MonoBehaviour("Player")
{
}

void Player::OnCollisionEnter(Collision &collision)
{

}

void Player::Start()
{
	// ���W�b�h
	regidbody_ = 
		game_object_.lock()->GetComponent<Rigidbody>();
	game_object = 
		game_object_.lock()->GetComponent<Object3dComponent>().lock()->GetObjectData();
}

void Player::FixedUpdate()
{
	// �L�[���͂ł̈ړ�����
	if (Input::GetKeyPress(DIK_W))
	{
		if (Input::GetKeyPress(DIK_LSHIFT))
		{

			regidbody_.lock()->AddForce(transform_.lock()->GetFront());
		}
		else
		{
			regidbody_.lock()->AddForce(transform_.lock()->GetFront());
		}

	}
	if (Input::GetKeyPress(DIK_S)) {
		// �ړ�
		regidbody_.lock()->AddForce(-transform_.lock()->GetFront());
	}
	if (Input::GetKeyPress(DIK_D))
	{
		// �ړ�
		regidbody_.lock()->AddForce(transform_.lock()->GetRight());
	}
	if (Input::GetKeyPress(DIK_A))
	{
		// �ړ�
		regidbody_.lock()->AddForce(-transform_.lock()->GetRight());
	}
}

void Player::Update()
{
	static float spin = 180;

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
		// �L�[���͂ł̈ړ�����
		if (Input::GetKeyPress(DIK_W)) {
			// �ړ�
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
			// �ړ�
			state = AnimationState::WALK_BACK;
			game_object->PlayAnimation(static_cast<int>(state));
		}
		else {
			if (Input::GetKeyPress(DIK_D)) {

				// �ړ�
				state = AnimationState::WALK_RIGHT;
				game_object->PlayAnimation(static_cast<int>(state));
			}
			if (Input::GetKeyPress(DIK_A)) {
				// �ړ�
				state = AnimationState::WALK_LEFT;
				game_object->PlayAnimation(static_cast<int>(state));
			}
		}
	}

	if (Input::GetKeyPress(DIK_Q))
	{
		spin -= 2;
		XMStoreFloat4(&transform_.lock()->localQuaternion, XMQuaternionRotationRollPitchYaw(0, spin * Mathf::deg_to_rad, 0));
	}
	if (Input::GetKeyPress(DIK_E))
	{
		spin += 2;
		XMStoreFloat4(&transform_.lock()->localQuaternion, XMQuaternionRotationRollPitchYaw(0, spin * Mathf::deg_to_rad, 0));
	}

	if (Input::GetKeyPressTrigger(DIK_SPACE)) {
		state = AnimationState::SLASH;
		game_object->PlayAnimation(static_cast<int>(state), false);
	}
}
