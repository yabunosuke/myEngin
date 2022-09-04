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
	if (Input::GetKeyPress(DIK_W))
	{
		if(Input::GetKeyPress(DIK_LSHIFT))
		{
			
			regidbody_.lock()->AddForce(transform_.lock()->GetFront() * 64);
		}
		else
		{
			regidbody_.lock()->AddForce(transform_.lock()->GetFront() * 8);
		}

	}
	if (Input::GetKeyPress(DIK_S)) {
		// �ړ�
		regidbody_.lock()->AddForce(-transform_.lock()->GetFront() * 8);
	}
	if(Input::GetKeyPress(DIK_D))
	{
		// �ړ�
		regidbody_.lock()->AddForce(transform_.lock()->GetRight() * 8);
	}
	if (Input::GetKeyPress(DIK_A))
	{
		// �ړ�
		regidbody_.lock()->AddForce(-transform_.lock()->GetRight() * 8);
	}
}

void PlayerTest::Update()
{
	static float spin = 180;
	
	//

	if (!(Input::GetKeyPress(DIK_W) ||
		Input::GetKeyPress(DIK_A) ||
		Input::GetKeyPress(DIK_S) ||
		Input::GetKeyPress(DIK_D)
		)) {
		if(state != AnimationState::SLASH ||
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
		if (Input::GetKeyPress(DIK_S)) {
			// �ړ�
			state = AnimationState::WALK_BACK;
			game_object->PlayAnimation(static_cast<int>(state));
		}
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

	if (Input::GetKeyPress(DIK_Q))
	{
		spin -= 2;
		XMStoreFloat4(&transform_.lock()->localQuaternion, XMQuaternionRotationRollPitchYaw(0,spin * Mathf::deg_to_rad,0));
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

	//if (Input::GetKeyPressTrigger(DIK_1)) {
	//	isDead = !isDead;
	//	if (isDead) {
	//		state = AnimationState::DETH;
	//		game_object->PlayAnimation(static_cast<int>(state), false);
	//	}
	//	else {
	//		state = AnimationState::IDLE;
	//	}
	//}

	//if(regidbody_.lock()->velocity_.Magnitude() > 3.0f)
	//{
	//}
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

