#include "GameScene.h"
#include "Object3dComponent.h"
#include "DirectXCommon.h"

GameScene::GameScene(IoChangedListener *impl)
	: AbstractScene(impl, "GameScene")
{
}

void GameScene::Initialize()
{
	// é
	auto castle = GameObject::CreateObject("Castle");
	castle->AddComponent<Object3dComponent>(
		DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
		"Assets/3d/Castle/Castle FBX.fbx");
}

void GameScene::Update()
{
	if (Input::GetButtonPressTrigger(GamePadButton::INPUT_A))
	{
		
	}
}
