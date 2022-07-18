#include "TitleScene.h"
#include "KeyboardInput.h"
#include "ModelManager.h"
#include "ImageManager.h"
#include "DirectXCommon.h"
#include "SphereCollider.h"
#include "CollisionManager.h"

#include "Collision.h"

#include "Easing.h"
#include "yMath.h"


#include "PipelineManager.h"

// コンポーネント
#include "Object3dComponent.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "Player.h"

TitleScene::TitleScene(IoChangedListener *impl)
	: AbstractScene(impl, "TitleScene")
{
	PipelineManager::GetInstance()->CreatePipline(DirectXCommon::dev.Get(), "Lambert");
	//test[0] = std::make_shared<Fbx>(DirectXCommon::dev.Get(), "Assets/3d/UNIT/plantune.fbx");


	gameObjectManager.CreateObject();
	auto plantune = gameObjectManager.CreateObject("plantune");
	plantune.lock().get()->AddComponent<Object3dComponent>(
			DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(), 
			"Assets/3d/UNIT/plantune.fbx");

	plantune.lock().get()->AddComponent<Player>();
}

void TitleScene::Initialize()
{
	cam = new Camera({ 0,0,-50 });
	Camera::SetCam(cam);

	gameObjectManager.Initialize();
}

void TitleScene::Finalize()
{
}

void TitleScene::Update()
{
	//test[0]->UpdateAnimation();
	
	//ゲームオブジェクト全てをアップデート
	gameObjectManager.Update();

	Camera::GetCam()->UpdateViewMatrix();
	Camera::GetCam()->UpdateProjectionMatrix();


 	CollisionManager::GetInstance()->CheckBroadCollisions();
}

void TitleScene::Draw() const
{
	gameObjectManager.Draw();


	Sprite::PreDraw(DirectXCommon::cmdList.Get());
	Sprite::PostDraw();
}

