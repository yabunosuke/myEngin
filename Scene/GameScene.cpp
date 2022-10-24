#include "GameScene.h"
#include "DirectXCommon.h"

#include "Math/Easing.h"
#include "Math/Mathf.h"

#include "Input.h"

#include "oldTexture.h"
#include "PipelineManager.h"
#include "PrimitiveRenderer.h"
#include "Object/GameObject/GameObject.h"

// コンポーネント
#include "Object3dComponent.h"
#include "Object/Component/Collider/SphereCollider/SphereCollider.h"
#include "Object/Component/Light.h"
#include "Object/Component/Rigidbody.h"
#include "Object/Component/Camera.h"
#include "Assets/Scripts/Player.h"
#include "Assets/Scripts/Enemy.h"
#include "Assets/Scripts/FlyEnemy.h"
#include "Assets/Scripts/CameraController.h"
#include "Weapon.h"
#include "Assets/Scripts/Drone.h"
#include "Object/Component/Collider/OBBCollider/OBBCollider.h"


GameScene::GameScene(IoChangedListener *impl)
	: AbstractScene(impl, "GameScene")
{
}

void GameScene::Initialize()
{

	// ライト
	auto directional_light_ = GameObject::CreateObject("Directional Light");
	directional_light_->AddComponent<Light>(LightType::Directional);
	XMStoreFloat4(&directional_light_->transform_->localQuaternion, XMQuaternionRotationRollPitchYaw(45, 45, 0));

	//// 城
	//auto castle = GameObject::CreateObject("Castle");
	//castle->AddComponent<Object3dComponent>(
	//	DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
	//	"Assets/3d/Castle/Castle FBX.fbx");
	//castle->transform_->localScale = { 20,20,20 };
	//castle->transform_->localPosition = { 0,-7,0 };

	//auto floors = GameObject::CreateObject("floors");
	//floors->AddComponent<OBBCollider>()
	//for( int i = 0; i<4;++i)
	{
		//for (int j = 0; j < 4; ++j)
		{
			auto floor = GameObject::CreateObject("Floor");
			floor->AddComponent<Object3dComponent>(
				DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
				"Assets/3d/Dungeon/fbx walls/floor_big.fbx");
			floor->AddComponent<OBBCollider>(Quaternion{ 0.0f,0.0f,0.0f,0.0f }, Vector3{ 1100.0f,1.0f,1100.0f });
			//floor->transform_->position = { i * 600.0f,0,j * 600.0f };
			//floor->SetParent(floors);
			//floors->AddComponent<OBBCollider>()
		}
	}

	// プレイヤー
	auto player = GameObject::CreateObject("Player");
	player->AddComponent<Object3dComponent>(
		DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
		"Assets/3d/sword human/Sword man.fbx");
	player->AddComponent<Rigidbody>();
	player->AddComponent<SphereCollider>(25, Vector3{ 0,20,0 });
	player->AddComponent<PlayerController>();
	XMStoreFloat4(&player->transform_->localQuaternion, XMQuaternionRotationRollPitchYaw(0, 0, 0));

	auto eye = GameObject::CreateObject("Eye");
	eye->SetParent(player);
	eye->transform_->localPosition = { 0.0f,150.0f,0.0f };

	auto drone = GameObject::CreateObject("Drone");
	drone->AddComponent<Object3dComponent>(
		DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
		"Assets/3d/Drone/drone.fbx");
	drone->AddComponent<Drone>(&player->transform_->position.r_);

	auto enemy = GameObject::CreateObject("Enemy");
	enemy->AddComponent<Object3dComponent>(
		DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
		"Assets/3d/Dwarf/Dwarf.fbx");
	enemy->AddComponent<SphereCollider>(25, Vector3{ 0,20,0 });
	enemy->AddComponent<Rigidbody>();
	enemy->transform_->scale = { 0.4f,0.4f,0.4f };
	enemy->transform_->localPosition = { 0.0f,0.0f,20.0f };
	enemy->AddComponent<Enemy>();

	srand(time(NULL));
	for (int i = 0; i < 2; ++i)
	{
		auto fly_enemy = GameObject::CreateObject("FlyEnemy");
		fly_enemy->AddComponent<Object3dComponent>(
			DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
			"Assets/3d/Suzy.fbx");
		auto collider = fly_enemy->AddComponent<SphereCollider>();
		fly_enemy->AddComponent<Rigidbody>();
		fly_enemy->AddComponent<FlyEnemy>();
		fly_enemy->transform_->position = {static_cast<float>(rand() % 1000 - 500),40.0f,static_cast<float>(rand() % 1000 - 500) };
		auto enemy_eye = GameObject::CreateObject("EnemyEye");
		auto collider_eye = enemy_eye->AddComponent<SphereCollider>(40, Vector3{ 0,20,0 });
		collider_eye->isTrigger = true;
		enemy_eye->SetParent(fly_enemy);
	}

	// カメラ
	auto camera = GameObject::CreateObject("CameraObject");
	camera->AddComponent<Camera>();
	camera->transform_->localPosition = { 0,200,-400 };
	XMStoreFloat4(&camera->transform_->localQuaternion, XMQuaternionRotationRollPitchYaw(0, 0, 0));
	camera->AddComponent<CameraController>(eye);
	//camera->SetParent(eye);

	// 武器
	auto weapon = GameObject::CreateObject("Weapon");
	weapon->AddComponent<Object3dComponent>(
		DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
		"Assets/3d/Leona's sword/Models and Textures/sword.fbx");
	weapon->SetParent(player);
	//game_object_manager_->SetPearentChild(player, weapon);
	weapon->AddComponent<Weapon>();
	auto weapon_collider = weapon->AddComponent<OBBCollider>(Quaternion{ 0.0f,0.0f,0.0f,0.0f },Vector3{8.0f,12.0f,8.0f});
	weapon_collider->isTrigger = true;
}

void GameScene::Update()
{
	AbstractScene::Update();
	if (Input::GetButtonPressTrigger(GamePadButton::INPUT_START))
	{
		implSceneChanged->SceneStackPop();
	}
}
