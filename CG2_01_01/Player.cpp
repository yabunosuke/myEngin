#include "Player.h"
#include "KeyboardInput.h"
#include "GameObject.h"
#include "Object3dComponent.h"

Player::Player() :
	Component("Player", true)
{

}

void Player::VirtualInitialize()
{
	transform_ = parent->GetComponent<TransformComponent>()->GetTransform();
	transform_->scale = 0.2f;
	
	game_object = parent->GetComponent<Object3dComponent>()->GetObjectData();

}

void Player::VirtualUpdate()
{
	//transform_ = parent->GetComponent<TransformComponent>()->GetTransform();
	
	Move();

	//game_object->PlayAnimation(static_cast<int>(state), isAnimationRoop);
}

void Player::VirtualLustUpdate()
{
	// 移動を適応
	// parent->GetComponent<TransformComponent>()->SetTransform(transform_);
}

void Player::VirtualDraw()
{
}

void Player::VirtualFinalize()
{
}

void Player::Infomation()
{

}

void Player::Move()
{
	if (!(KeyboardInput::GetIns()->GetKeyPress(DIK_W) ||
		KeyboardInput::GetIns()->GetKeyPress(DIK_A) ||
		KeyboardInput::GetIns()->GetKeyPress(DIK_S) ||
		KeyboardInput::GetIns()->GetKeyPress(DIK_D)
		)) {
		if (state != AnimationState::DETH) {
			if (isRifle) {
				if (!(state == AnimationState::PULL_RIFLE &&
					game_object->IsPlayAnimation())) {

					state = AnimationState::IDLE_LIFRE;
					game_object->PlayAnimation(static_cast<int>(state));
				}
			}
			else {
				if (!(state == AnimationState::PUT_RIFLE &&
					game_object->IsPlayAnimation())) {

					state = AnimationState::IDLE;
					game_object->PlayAnimation(static_cast<int>(state));
				}
			}
		}
		
	}
	else {
		if (KeyboardInput::GetIns()->GetKeyPress(DIK_W)) {
			transform_->Translate(0, 0, 0.7f);
			if (isRifle) {
				state = AnimationState::RIFLE_WALK;
			}
			else {
				state = AnimationState::WALK;
			}
			game_object->PlayAnimation(static_cast<int>(state));
		}
		if (KeyboardInput::GetIns()->GetKeyPress(DIK_S)) {
			transform_->Translate(0, 0, -0.5f);
			if (isRifle) {
				state = AnimationState::RIFLE_WALK_BACK;
			}
			else {
				state = AnimationState::WALK_BACK;
			}
			game_object->PlayAnimation(static_cast<int>(state));
		}
		if (KeyboardInput::GetIns()->GetKeyPress(DIK_D)) {
			transform_->Translate(1.8f, 0, 0);
			if (isRifle) {
				state = AnimationState::RIFLE_WALK_RIGHT;
			}
			else {
				state = AnimationState::WALK_RIGHT;
			}
			game_object->PlayAnimation(static_cast<int>(state));
		}
		if (KeyboardInput::GetIns()->GetKeyPress(DIK_A)) {
			transform_->Translate(-1.8f, 0, 0);
			if (isRifle) {
				state = AnimationState::RIFLE_WALK_LEFT;
			}
			else {
				state = AnimationState::WALK_LEFT;
			}
			game_object->PlayAnimation(static_cast<int>(state));
		}
	}

	

	// 回転デバッグ
	if (KeyboardInput::GetIns()->GetKeyPress(DIK_Q)) {
		transform_->rotate.y -= 1.0f;
	}
	if (KeyboardInput::GetIns()->GetKeyPress(DIK_E)) {
		transform_->rotate.y += 1.0f;

	}

	if (KeyboardInput::GetIns()->GetKeyPressT(DIK_SPACE)) {
		isRifle = !isRifle;
		if (isRifle) {
			state = AnimationState::PULL_RIFLE;
			game_object->PlayAnimation(static_cast<int>(state),false);
		}
		else {
			state = AnimationState::PUT_RIFLE;
			game_object->PlayAnimation(static_cast<int>(state), false);
		}
	}

	if (KeyboardInput::GetIns()->GetKeyPressT(DIK_1)) {
		isDead = !isDead;
		if (isDead) {
			state = AnimationState::DETH;
			game_object->PlayAnimation(static_cast<int>(state), false);
		}
		else {
			state = AnimationState::IDLE;
		}
	}
	

}
