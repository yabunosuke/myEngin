#include "GameScene.h"

GameScene::GameScene(IoChangedListener *impl)
	: AbstractScene(impl, "GameScene")
{
}

void GameScene::Update()
{
	if (Input::GetButtonPressTrigger(GamePadButton::INPUT_A))
	{
		
	}
}
