#include "PlayerTest.h"
#include "Object/GameObject/GameObject.h"
#include "Object3dComponent.h"
PlayerTest::PlayerTest():
	ScriptComponent("Player")
{
}

void PlayerTest::OnCollisionEnter()
{
	game_object->SetColor({1,0,0,1});

}

void PlayerTest::Initialize()
{
	// �g�����X�t�H�[��

	// ���W�b�h
	regidbody_ = game_object_->GetComponent<Rigidbody>();

	// �I�u�W�F�N�g�f�[�^
	game_object = game_object_->GetComponent<Object3dComponent>().lock()->GetObjectData();


}

void PlayerTest::FixedUpdate()
{
	
	// �L�[���͂ł̈ړ�����
	if (KeyboardInput::GetIns()->GetKeyPress(DIK_W))
	{
		regidbody_.lock()->AddForce(transform_.lock()->GetFront());

	}
	if (KeyboardInput::GetIns()->GetKeyPress(DIK_S)) {
		// �ړ�
		regidbody_.lock()->AddForce(-transform_.lock()->GetFront());
	}
	if(KeyboardInput::GetIns()->GetKeyPress(DIK_D))
	{
		// �ړ�
		regidbody_.lock()->AddForce(transform_.lock()->GetRight());
	}
	if (KeyboardInput::GetIns()->GetKeyPress(DIK_A))
	{
		// �ړ�
		regidbody_.lock()->AddForce(-transform_.lock()->GetRight());
	}
}

void PlayerTest::Update()
{
	static float spin = 180;
	
	

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
		// �L�[���͂ł̈ړ�����
		if (KeyboardInput::GetIns()->GetKeyPress(DIK_W)) {
			// �ړ�
			if (isRifle) {
				state = AnimationState::RIFLE_WALK;
			}
			else {
				state = AnimationState::WALK;
			}
			game_object->PlayAnimation(static_cast<int>(state));
		}
		if (KeyboardInput::GetIns()->GetKeyPress(DIK_S)) {
			// �ړ�
			if (isRifle) {
				state = AnimationState::RIFLE_WALK_BACK;
			}
			else {
				state = AnimationState::WALK_BACK;
			}
			game_object->PlayAnimation(static_cast<int>(state));
		}
		if (KeyboardInput::GetIns()->GetKeyPress(DIK_D)) {
			
			if (isRifle) {
				state = AnimationState::RIFLE_WALK_RIGHT;
			}
			else {
				state = AnimationState::WALK_RIGHT;
			}
			game_object->PlayAnimation(static_cast<int>(state));
		}
		if (KeyboardInput::GetIns()->GetKeyPress(DIK_A)) {
			if (isRifle) {
				state = AnimationState::RIFLE_WALK_LEFT;
			}
			else {
				state = AnimationState::WALK_LEFT;
			}
			game_object->PlayAnimation(static_cast<int>(state));
		}
	}
	
	

	if (KeyboardInput::GetIns()->GetKeyPress(DIK_Q))
	{
		spin -= 2;
		XMStoreFloat4(&transform_.lock()->localQuaternion, XMQuaternionRotationRollPitchYaw(0,spin * Mathf::deg_to_rad,0));
	}
	if (KeyboardInput::GetIns()->GetKeyPress(DIK_E))
	{
		spin += 2;
		XMStoreFloat4(&transform_.lock()->localQuaternion, XMQuaternionRotationRollPitchYaw(0, spin * Mathf::deg_to_rad, 0));
	}

	if (KeyboardInput::GetIns()->GetKeyPressT(DIK_SPACE)) {
		isRifle = !isRifle;
		if (isRifle) {
			state = AnimationState::PULL_RIFLE;
			game_object->PlayAnimation(static_cast<int>(state), false);
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

	if(regidbody_.lock()->velocity_.Magnitude() > 3.0f)
	{
	}
}

void PlayerTest::LustUpdate()
{
}

void PlayerTest::Draw()
{
}

void PlayerTest::Finalize()
{
}

