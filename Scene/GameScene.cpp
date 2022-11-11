#include "GameScene.h"
#include "DirectXCommon.h"

#include "Math/Easing.h"
#include "Math/Mathf.h"

#include "Input.h"

#include "Texture.h"
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
#include "Assets/Scripts/FlyEnemyEye.h"
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
	auto floors = GameObject::CreateObject("Floors");
	floors->tag = "Floor";
	floors->isStatic = true;
	floors->AddComponent<OBBCollider>(Quaternion{ 0.0f,0.0f,0.0f,0.0f }, Vector3{ 18.5f,1.0f,18.5f }, Vector3{ 18.5f / 2.0f,-0.5f,18.5f / 2.0f });

	for( int i = 0; i<4;++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			auto floor2_big = GameObject::CreateObject("Floor2 big");
			floor2_big->AddComponent<Object3dComponent>(
				DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
				"Assets/3d/Dungeon/fbx walls/floor2_big.fbx");
			//floor2_big->AddComponent<OBBCollider>(Quaternion{ 0.0f,0.0f,0.0f,0.0f }, Vector3{ 6.0f,1.0f,6.0f }, Vector3{ -3.0f,-0.5f,-3.0f });
			floor2_big->transform_->position = { i * 6.0f,0.0f,j * 6.0f };
			floor2_big->SetParent(floors);
		}
	}

	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			if (i == 0 && j == 0)continue;;
			auto pillar_big = GameObject::CreateObject("Pillar big");
			pillar_big->tag = "Pillar";
			pillar_big->isStatic = true;
			pillar_big->AddComponent<Object3dComponent>(
				DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
				"Assets/3d/Dungeon/fbx walls/pillar_big.fbx");
			pillar_big->AddComponent<OBBCollider>(Quaternion{ 0,0,0,0 }, Vector3{ 1.0f,4.5f,1.0f }, Vector3{ 0,2.25f,0 });
			pillar_big->transform_->position = { i * 40.0f / 3.0f,-0.2f,j * 40.0f / 3.0f }; 
		}
	
	}

	for (int i = 0; i < 4; ++i)
	{
		auto wall = GameObject::CreateObject("Wall");
		wall->AddComponent<OBBCollider>(Quaternion{ 0,0,0,0 }, Vector3{ 0.5f,4.5f,6.0f }, Vector3{ 0.25f,2.25f,-3 });
		wall->isStatic = true;
		wall->transform_->position = { -6.0f,-0.2f,i * 6.0f };
		wall->tag = "Wall";
		wall->isStatic = true;
		wall->AddComponent<Object3dComponent>(
			DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
			"Assets/3d/Dungeon/fbx walls/Wall.fbx");
	}
	for (int i = 0; i < 4; ++i)
	{
		auto wall = GameObject::CreateObject("Wall");
		wall->AddComponent<OBBCollider>(Quaternion{ 0,0,0,0 }, Vector3{ 0.5f,4.5f,6.0f }, Vector3{ 0.25f,2.25f,-3 });
		wall->isStatic = true;
		wall->transform_->position = { 18.0f,-0.2f,i * 6.0f };
		wall->tag = "Wall";
		wall->isStatic = true;
		wall->AddComponent<Object3dComponent>(
			DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
			"Assets/3d/Dungeon/fbx walls/Wall.fbx");
	}
	for (int i = 0; i < 4; ++i)
	{
		auto wall = GameObject::CreateObject("Wall");
		wall->AddComponent<OBBCollider>(Quaternion{ 0,0,0,0 }, Vector3{ 0.5f,4.5f,6.0f }, Vector3{ 0.25f,2.25f,-3 });
		wall->isStatic = true;
		wall->transform_->position = { i * 6.0f -6.0f ,-0.2f,-6.0};
		wall->transform_->quaternion = Quaternion::Euler(0, -90.0f * Mathf::deg_to_rad, 0);
		wall->tag = "Wall";
		wall->isStatic = true;
		wall->AddComponent<Object3dComponent>(
			DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
			"Assets/3d/Dungeon/fbx walls/Wall.fbx");
	}

	for (int i = 0; i < 2; ++i)
	{
		auto wall = GameObject::CreateObject("Wall");
		wall->AddComponent<OBBCollider>(Quaternion{ 0,0,0,0 }, Vector3{ 0.5f,4.5f,6.0f }, Vector3{ 0.25f,2.25f,-3 });
		wall->isStatic = true;
		wall->transform_->position = { i * 6.0f - 6.0f ,-0.2f,18.0 };
		wall->transform_->quaternion = Quaternion::Euler(0, -90.0f * Mathf::deg_to_rad, 0);
		wall->tag = "Wall";
		wall->isStatic = true;
		wall->AddComponent<Object3dComponent>(
			DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
			"Assets/3d/Dungeon/fbx walls/Wall.fbx");
	}


	// プレイヤー
	auto player = GameObject::CreateObject("Player");
	player->AddComponent<Object3dComponent>(
		DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
		"Assets/3d/sword human/Sword man.fbx");
	auto rg = player->AddComponent<Rigidbody>();
	rg->useGravity = true;
	player->AddComponent<PlayerController>();
	player->AddComponent<SphereCollider>(1, Vector3{ 0,0.5f,0 });
	auto player_light = GameObject::CreateObject("Player Light");
	player_light->transform_->localPosition = { 0.0f,1.0f,0.0f };
	auto ligh_info = player_light->AddComponent<Light>();
	ligh_info->range = 1.0f;
	ligh_info->intensity = 1.0f;
	player_light->SetParent(player);

	XMStoreFloat4(&player->transform_->localQuaternion, XMQuaternionRotationRollPitchYaw(0, 0, 0));

	auto eye = GameObject::CreateObject("Eye");
	eye->transform_->localPosition = {0, 1.4f, 0.0f};
	eye->SetParent(player);

	auto drone = GameObject::CreateObject("Drone");
	drone->AddComponent<Object3dComponent>(
		DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
		"Assets/3d/Drone/drone.fbx");
	drone->AddComponent<Drone>(&player->transform_->position.r_);

	/*auto enemy = GameObject::CreateObject("Enemy");
	enemy->AddComponent<Object3dComponent>(
		DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
		"Assets/3d/Dwarf/Dwarf.fbx");
	enemy->AddComponent<SphereCollider>(1, Vector3{ 0,0.7f,0 });
	auto enemy_rg = enemy->AddComponent<Rigidbody>();
	enemy_rg->useGravity = true;
	enemy->transform_->position = { 8.0f,0.0f,6.0f };
	enemy->AddComponent<Enemy>();*/

	/*srand(time(NULL));
	for (int i = 0; i < 3; ++i)
	{
		auto fly_enemy = GameObject::CreateObject("FlyEnemy");
		fly_enemy->AddComponent<Object3dComponent>(
			DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
			"Assets/3d/Suzy.fbx");
		auto collider = fly_enemy->AddComponent<SphereCollider>(0.01f,Vector3{0,1,0});
		fly_enemy->AddComponent<Rigidbody>();
		fly_enemy->AddComponent<FlyEnemy>();
		fly_enemy->transform_->position = {static_cast<float>(rand() % 10 - 5),5.0f,static_cast<float>(rand() % 10 - 5) };
		auto light = fly_enemy->AddComponent<Light>();
		light->color = { 1,0,0,1 };
		light->range = 2.0f;
		light->intensity = 1.0f;
		auto enemy_eye = GameObject::CreateObject("EnemyEye");
		enemy_eye->AddComponent<FlyEnemyEye>();
		auto collider_eye = enemy_eye->AddComponent<SphereCollider>(4, Vector3{ 0,2,0 });
		collider_eye->isTrigger = true;
		enemy_eye->SetParent(fly_enemy);
	}*/

	// カメラ
	auto camera = GameObject::CreateObject("CameraObject");
	camera->AddComponent<Camera>();
	camera->transform_->localPosition = { 0,200,-400 };
	XMStoreFloat4(&camera->transform_->localQuaternion, XMQuaternionRotationRollPitchYaw(0, 0, 0));
	camera->AddComponent<CameraController>(eye);
	//camera->SetParent(eye);

	// 武器
	auto weapon = GameObject::CreateObject("Weapon");
	weapon->tag = "Weapon";
	weapon->AddComponent<Object3dComponent>(
		DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
		"Assets/3d/Leona's sword/Models and Textures/sword.fbx");
	weapon->SetParent(player);
	//game_object_manager_->SetPearentChild(player, weapon);
	weapon->AddComponent<Weapon>();
	auto weapon_collider = weapon->AddComponent<OBBCollider>(Quaternion{ 0.0f,0.0f,0.0f,0.0f },Vector3{1.8f,6.2f,1.8f});
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
