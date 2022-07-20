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
//#include "Quaternion.h"


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


	auto house = game_object_manager_.CreateObject("house");
	house.lock().get()->AddComponent<Object3dComponent>(
		DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
		"Assets/3d/UNIT/plantune.fbx");
	

	auto player = game_object_manager_.CreateObject("human");
	player.lock().get()->AddComponent<Object3dComponent>(
			DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(), 
			//"Assets/3d/UNIT/cube.000.fbx");
			"Assets/3d/test/human.fbx");
			//"Assets/3d/Test/stage.fbx");

	player.lock().get()->AddComponent<Player>();
	player.lock().get()->AddComponent<ColliderComponent>(this);
}

void TitleScene::Initialize()
{
	cam = new Camera({ 0,0,50 });
	Camera::SetCam(cam);

	game_object_manager_.Initialize();
}

void TitleScene::Finalize()
{
}

void TitleScene::Update()
{
	//test[0]->UpdateAnimation();
	
	//ゲームオブジェクト全てをアップデート
	game_object_manager_.Update();

	//当たり判定チェック
	collision_manager_.CheckBroadCollisions(game_object_manager_.gameObjects);

	game_object_manager_.LastUpdate();

	Camera::GetCam()->UpdateViewMatrix();
	Camera::GetCam()->UpdateProjectionMatrix();


 	
}

void TitleScene::Draw() const
{
	game_object_manager_.Draw();


	Sprite::PreDraw(DirectXCommon::cmdList.Get());
	Sprite::PostDraw();
}

