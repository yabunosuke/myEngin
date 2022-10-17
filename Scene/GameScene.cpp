#include "GameScene.h"
#include "Object3dComponent.h"
#include "DirectXCommon.h"
#include "Object/Component/Camera.h"

GameScene::GameScene(IoChangedListener *impl)
	: AbstractScene(impl, "GameScene")
{
}

void GameScene::Initialize()
{

	// ƒJƒƒ‰
	auto camera = GameObject::CreateObject("CameraObject");
	camera->AddComponent<Camera>();
	camera->transform_->localPosition = { 0,200,-400 };
	// é
	auto castle = GameObject::CreateObject("Castle");
	castle->AddComponent<Object3dComponent>(
		DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
		"Assets/3d/Castle/Castle FBX.fbx");
}

void GameScene::Update()
{
	AbstractScene::Update();
	if (Input::GetButtonPressTrigger(GamePadButton::INPUT_X))
	{
		implSceneChanged->SceneStackPop();
	}
}
