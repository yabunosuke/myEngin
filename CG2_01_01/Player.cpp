#include "Player.h"
#include "KeyboardInput.h"
#include "TransformComponent.h"
#include "GameObject.h"

Player::Player() :
	Component("Player", true)
{
}

void Player::VirtualInitialize()
{
}

void Player::VirtualUpdate()
{
	XMFLOAT3 pos = parent->GetComponent<TransformComponent>()->GetPositon();
	velosity *= 0.95f;
	if (KeyboardInput::GetIns()->GetKeyPress(DIK_W)) {
		pos.z += 1.0f;
	}
	if (KeyboardInput::GetIns()->GetKeyPress(DIK_S)) {
		pos.z -= 1.0f;
	}
	if (KeyboardInput::GetIns()->GetKeyPress(DIK_D)) {
		pos.x += 1.0f;
	}
	if (KeyboardInput::GetIns()->GetKeyPress(DIK_A)) {
		pos.x -= 1.0f;
	}

	parent->GetComponent<TransformComponent>()->SetPositon(pos);
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
