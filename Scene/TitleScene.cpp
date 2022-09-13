#include "TitleScene.h"
#include "DirectXCommon.h"
#include "SphereCollider.h"
#include "CollisionManager.h"

#include "Math/Easing.h"
#include "Math/Mathf.h"
//#include "Quaternion.h"

#include "Texture.h"
#include "PipelineManager.h"
#include "PrimitiveRenderer.h"

// コンポーネント
#include "Object3dComponent.h"
#include "Component/ColliderComponent.h"
#include "Object/Component/Light.h"
#include "Object/Component/Rigidbody.h"
#include "Object/Component/Camera.h"
#include "Weapon.h"
#include "Object/Component/Renderer/MeshRenderer/MeshRenderer.h"
#include "PlayerTest.h"


// テスト
#include "testObject.h"
#include "newObject/GameObject/testGameObject.h"

TitleScene::TitleScene(IoChangedListener *impl)
	: AbstractScene(impl, "TitleScene")
{

	// プレイヤー
	auto player = game_object_manager_.CreateObject("Human");
	player.lock().get()->AddComponent<Object3dComponent>(
		DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
		"Assets/3d/sword human/Sword man.fbx");
	player.lock().get()->AddComponent<ColliderComponent>(this);
	player.lock().get()->AddComponent<Rigidbody>();
	player.lock().get()->AddComponent<PlayerTest>();
	player.lock()->transform->lock()->localScale = { 0.4f,0.4f,0.4f };

	auto player_light = game_object_manager_.CreateObject("Player light");
	player_light.lock().get()->AddComponent<Light>(light_manager_);
	player_light.lock()->transform->lock()->localPosition = { 0.0f,50.0f,0.0f };
	game_object_manager_.SetPearentChild(player, player_light);

	auto castle = game_object_manager_.CreateObject("Castle");
	castle.lock().get()->AddComponent<Object3dComponent>(
		DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
		"Assets/3d/Castle/Castle FBX.fbx");
	castle.lock()->transform->lock()->localScale = { 20,20,20 };
	castle.lock()->transform->lock()->localPosition = { 0,-7,0 };

	auto danbo = game_object_manager_.CreateObject("danbo");
	danbo.lock()->AddComponent<Object3dComponent>(
		DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
		"Assets/3d/UNIT/danbo_fbx/danbo_taiki.fbx");
	danbo.lock().get()->AddComponent<ColliderComponent>(this, CollisionShapeType::SHAPE_SPHERE);

	auto camera = game_object_manager_.CreateObject("Camera");
	camera.lock()->AddComponent<Camera>();
	camera.lock()->transform->lock()->localPosition = { 0,200,-400 };
	XMStoreFloat4(&camera.lock()->transform->lock()->localQuaternion, XMQuaternionRotationRollPitchYaw(0, 0, 0));
	game_object_manager_.SetPearentChild(player, camera);



	// 武器
	auto weapon = game_object_manager_.CreateObject("Leona's sword");
	weapon.lock()->AddComponent<Object3dComponent>(
		DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
		"Assets/3d/Leona's sword/Models and Textures/sword.fbx");
	game_object_manager_.SetPearentChild(player, weapon);
	weapon.lock()->AddComponent<Weapon>();
	weapon.lock()->AddComponent<ColliderComponent>(this, CollisionShapeType::SHAPE_SPHERE);


	// テスト
	auto test = testObject::CreateObject<testGameObject>();
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
	collision_manager_.CheckBroadCollisions(game_object_manager_.game_objects_);

	game_object_manager_.LastUpdate();
	
 	
}